#include "Encryptor.h"
#include "EACglobals.h"
#include "generators/IRndGen.h"

Encryptor::Encryptor()
    : m_cipher(CaesarInterface::getCaesarFunction(pCaesarSettings->algorithm,
                                  pCaesarSettings->limitAlgRounds ? pCaesarSettings->algorithmRoundsCount : -1)),
      m_key(new bits_t[pCaesarSettings->keyLength]),
      m_ad(new bits_t[pCaesarSettings->adLength]),
      m_smn(new bits_t[pCaesarSettings->smnLength]),
      m_pmn(new bits_t[pCaesarSettings->pmnLength]),
      m_plaintext(new bits_t[pCaesarSettings->plaintextLength]),
      // decrypted plaintext is one block bigger, in case the cipher creates a new block by padding
      m_decryptedPlaintext(new bits_t[pCaesarSettings->plaintextLength+32]),
      m_decryptedSmn(new bits_t[pCaesarSettings->smnLength]),
      m_decryptedPlaintextLength(0),
      m_setup(false) {
    pCaesarSettings->ciphertextLength = pCaesarSettings->plaintextLength + pCaesarSettings->cipertextOverhead;
}

Encryptor::~Encryptor() {
    if (m_cipher) { delete m_cipher; m_cipher = NULL; }
    if (m_key) { delete[] m_key; m_key = NULL; }
    if (m_ad) { delete[] m_ad; m_ad = NULL; }
    if (m_smn) { delete[] m_smn; m_smn = NULL; }
    if (m_pmn) { delete[] m_pmn; m_pmn = NULL; }
    if (m_plaintext) { delete[] m_plaintext; m_plaintext = NULL; }
    if (m_decryptedPlaintext) { delete[] m_decryptedPlaintext; m_decryptedPlaintext = NULL; }
    if (m_decryptedSmn) { delete[] m_decryptedSmn; m_decryptedSmn = NULL; }
}

int Encryptor::initArray(bits_t* data, length_t dataLength, int dataType) {
    switch (dataType) {
    case CAESAR_TYPE_ZEROS:
    case CAESAR_TYPE_COUNTER:
        memset(data, 0, dataLength);
        break;
    case CAESAR_TYPE_RANDOM_ONCE:
    case CAESAR_TYPE_RANDOM:
        for (length_t i = 0; i < dataLength; i++) { rndGen->getRandomFromInterval(255, &(data[i])); }
        break;
    default:
        mainLogger.out(LOGGER_ERROR) << "Unknown data type (" << dataType << ")." << endl;
        return STAT_PROJECT_ERROR;
    }
    return STAT_OK;
}

void Encryptor::increaseArray(bits_t* data, length_t dataLength) {
    for (length_t i = 0; i < dataLength; i++) {
        if (data[i] != UCHAR_MAX) {
            data[i]++;
            return;
        }
        data[i] = 0;
    }
}

int Encryptor::setup() {
    if (m_cipher == NULL) { return STAT_PROJECT_ERROR; }
    int status = STAT_OK;
    status = initArray(m_ad, pCaesarSettings->adLength, pCaesarSettings->adType);
    if (status != STAT_OK) { return status; }
    status = initArray(m_key, pCaesarSettings->keyLength, pCaesarSettings->keyType);
    if (status != STAT_OK) { return status; }
    status = initArray(m_smn, pCaesarSettings->smnLength, pCaesarSettings->smnType);
    if (status != STAT_OK) { return status; }
    status = initArray(m_pmn, pCaesarSettings->pmnLength, pCaesarSettings->pmnType);
    if (status != STAT_OK) { return status; }
    status = initArray(m_plaintext, pCaesarSettings->plaintextLength, pCaesarSettings->plaintextType);
    if (status != STAT_OK) { return status; }
    m_setup = true;
    return status;
}

int Encryptor::update() {
    // update counters
    if (pCaesarSettings->adType == CAESAR_TYPE_COUNTER) { increaseArray(m_ad, pCaesarSettings->adLength); }
    if (pCaesarSettings->keyType == CAESAR_TYPE_COUNTER) { increaseArray(m_key, pCaesarSettings->keyLength); }
    if (pCaesarSettings->smnType == CAESAR_TYPE_COUNTER) { increaseArray(m_smn, pCaesarSettings->smnLength); }
    if (pCaesarSettings->pmnType == CAESAR_TYPE_COUNTER) { increaseArray(m_pmn, pCaesarSettings->pmnLength); }
    if (pCaesarSettings->plaintextType == CAESAR_TYPE_COUNTER) { increaseArray(m_plaintext, pCaesarSettings->adLength); }
    // update randoms
    if (pCaesarSettings->adType == CAESAR_TYPE_RANDOM) { initArray(m_ad, pCaesarSettings->adLength, CAESAR_TYPE_RANDOM); }
    if (pCaesarSettings->keyType == CAESAR_TYPE_RANDOM) { initArray(m_key, pCaesarSettings->keyLength, CAESAR_TYPE_RANDOM); }
    if (pCaesarSettings->smnType == CAESAR_TYPE_RANDOM) { initArray(m_smn, pCaesarSettings->smnLength, CAESAR_TYPE_RANDOM); }
    if (pCaesarSettings->pmnType == CAESAR_TYPE_RANDOM) { initArray(m_pmn, pCaesarSettings->pmnLength, CAESAR_TYPE_RANDOM); }
    if (pCaesarSettings->plaintextType == CAESAR_TYPE_RANDOM) { initArray(m_plaintext, pCaesarSettings->adLength, CAESAR_TYPE_RANDOM); }
    return STAT_OK;
}

int Encryptor::encrypt(bits_t *c, length_t *clen) {
    if (!m_setup) {
        mainLogger.out(LOGGER_ERROR) << "Cipher not properly setup!" << endl;
        return STAT_PROJECT_ERROR;
    }

    int encryptionStatus = 0;
    encryptionStatus = m_cipher->encrypt(c, clen, m_plaintext, pCaesarSettings->plaintextLength,
                                         m_ad, pCaesarSettings->adLength, m_smn, m_pmn, m_key);
    if (encryptionStatus != 0) {
        mainLogger.out(LOGGER_ERROR) << "Encryption failed (status " << encryptionStatus << ")." << endl;
        return STAT_PROJECT_ERROR;
    }
    encryptionStatus = m_cipher->decrypt(m_decryptedPlaintext, &m_decryptedPlaintextLength, m_decryptedSmn,
                                         c, *clen, m_ad, pCaesarSettings->adLength, m_pmn, m_key);
    if (encryptionStatus != 0) {
        mainLogger.out(LOGGER_ERROR) << "Decryption failed (status " << encryptionStatus << ")." << endl;
        return STAT_PROJECT_ERROR;
    }

    // save debug information if required
    if (pGlobals->settings->outputs.verbosity >= LOGGER_VERBOSITY_DEEP_DEBUG) {
        ofstream tvFile;
        tvFile.open(FILE_TEST_VECTORS_HR, ios_base::app);
        if (!tvFile.is_open()) {
            mainLogger.out(LOGGER_ERROR) << "Cannot write file for test vectors (" << FILE_TEST_VECTORS_HR << ")." << endl;
            return STAT_FILE_WRITE_FAIL;
        }
        tvFile << "plain:     ";
        for (length_t byte = 0; byte < pCaesarSettings->plaintextLength; byte++) {
            tvFile << hex << setfill('0') << setw(2) << (int) m_plaintext[byte];
        }
        tvFile << endl << "key:       ";
        for (length_t byte = 0; byte < pCaesarSettings->keyLength; byte++) {
            tvFile << hex << setfill('0') << setw(2) << (int) m_key[byte];
        }
        tvFile << endl << "ad:        ";
        for (length_t byte = 0; byte < pCaesarSettings->adLength; byte++) {
            tvFile << hex << setfill('0') << setw(2) << (int) m_ad[byte];
        }
        tvFile << endl << "smn:       ";
        for (length_t byte = 0; byte < pCaesarSettings->smnLength; byte++) {
            tvFile << hex << setfill('0') << setw(2) << (int) m_smn[byte];
        }
        tvFile << endl << "pmn:       ";
        for (length_t byte = 0; byte < pCaesarSettings->pmnLength; byte++) {
            tvFile << hex << setfill('0') << setw(2) << (int) m_pmn[byte];
        }
        tvFile << endl << "encrypted: ";
        for (length_t byte = 0; byte < *clen; byte++) {
            tvFile << hex << setfill('0') << setw(2) << (int) c[byte];
        }
        tvFile << endl << "decrypted: ";
        for (length_t byte = 0; byte < m_decryptedPlaintextLength; byte++) {
            tvFile << hex << setfill('0') << setw(2) << (int)m_decryptedPlaintext[byte];
        }
        tvFile << endl << "dec-smn:   ";
        for (length_t byte = 0; byte < pCaesarSettings->smnLength; byte++) {
            tvFile << hex << setfill('0') << setw(2) << (int)m_decryptedSmn[byte];
        }
        tvFile << endl << "---" << endl;
        if (tvFile.fail()) {
            mainLogger.out(LOGGER_ERROR) << "Problem when saving test vectors (" << FILE_TEST_VECTORS_HR << ")." << endl;
            return STAT_FILE_WRITE_FAIL;
        }
        tvFile.close();
    }

    // veryfing decryption
    if (pCaesarSettings->plaintextLength != m_decryptedPlaintextLength) {
        mainLogger.out(LOGGER_ERROR) << "Decrypted plaintext length mismatch (" << pCaesarSettings->plaintextLength << " versus " << m_decryptedPlaintextLength << ")." << endl;
        return STAT_PROJECT_ERROR;
    }
    if (memcmp(m_plaintext, m_decryptedPlaintext, m_decryptedPlaintextLength) != 0) {
        mainLogger.out(LOGGER_ERROR) << "Decrypted message mismatch." << endl;
        return STAT_PROJECT_ERROR;
    }
    if (memcmp(m_smn, m_decryptedSmn, pCaesarSettings->smnLength) != 0) {
        mainLogger.out(LOGGER_ERROR) << "Decrypted secret message number mismatch." << endl;
        return STAT_PROJECT_ERROR;
    }

    return STAT_OK;
}

string Encryptor::shortDescription() {
    if (m_cipher == NULL) {
        return "<no cipher allocated>";
    }
    string desription =  m_cipher->shortDescription() + ", ";
    if (pCaesarSettings->limitAlgRounds) {
        desription += CommonFnc::toString(pCaesarSettings->algorithmRoundsCount) + " rounds";
    } else {
        desription += "unlimited version";
    }
    return desription;
}

int Encryptor::saveState(TiXmlNode* pRoot) const {
    TiXmlElement* pElem = NULL;
    ostringstream ss;
    // save algorithm details
    pElem = new TiXmlElement("algorithm");
    pElem->LinkEndChild(new TiXmlText(CommonFnc::toString(pCaesarSettings->algorithm).c_str()));
    pElem->SetAttribute("description", m_cipher->shortDescription().c_str());
    if (pCaesarSettings->limitAlgRounds) {
        pElem->SetAttribute("rounds", CommonFnc::toString(pCaesarSettings->algorithmRoundsCount).c_str());
    }
    pRoot->LinkEndChild(pElem);
    // save plaintext
    pElem = new TiXmlElement("plaintext");
    pElem->LinkEndChild(new TiXmlText(CommonFnc::arrayToHexa(m_plaintext,
        static_cast<unsigned int>(pCaesarSettings->plaintextLength)).c_str()));
    pRoot->LinkEndChild(pElem);
    // save key
    pElem = new TiXmlElement("key");
    pElem->LinkEndChild(new TiXmlText(CommonFnc::arrayToHexa(m_key,
        static_cast<unsigned int>(pCaesarSettings->keyLength)).c_str()));
    pRoot->LinkEndChild(pElem);
    // save associated data (ad)
    pElem = new TiXmlElement("ad");
    pElem->LinkEndChild(new TiXmlText(CommonFnc::arrayToHexa(m_ad,
        static_cast<unsigned int>(pCaesarSettings->adLength)).c_str()));
    pRoot->LinkEndChild(pElem);
    // save secret message number (smn)
    pElem = new TiXmlElement("smn");
    pElem->LinkEndChild(new TiXmlText(CommonFnc::arrayToHexa(m_smn,
        static_cast<unsigned int>(pCaesarSettings->smnLength)).c_str()));
    pRoot->LinkEndChild(pElem);
    // save public message number (pmn)
    pElem = new TiXmlElement("pmn");
    pElem->LinkEndChild(new TiXmlText(CommonFnc::arrayToHexa(m_pmn,
        static_cast<unsigned int>(pCaesarSettings->pmnLength)).c_str()));
    pRoot->LinkEndChild(pElem);

    return STAT_OK;
}

int Encryptor::loadState(TiXmlNode* pRoot) {
    if (m_cipher == NULL) { return STAT_PROJECT_ERROR; }
    int status = STAT_OK;
    if (atoi(getXMLElementValue(pRoot, "algorithm").c_str()) != pCaesarSettings->algorithm) {
        mainLogger.out(LOGGER_ERROR) << "Incompatible algorithm types." << endl;
        return STAT_CONFIG_INCORRECT;
    }
    status = CommonFnc::hexaToArray(getXMLElementValue(pRoot, "plaintext"), 
        static_cast<unsigned int>(pCaesarSettings->plaintextLength), m_plaintext);
    if (status != STAT_OK) return status;
    status = CommonFnc::hexaToArray(getXMLElementValue(pRoot, "key"), 
        static_cast<unsigned int>(pCaesarSettings->keyLength), m_key);
    if (status != STAT_OK) return status;
    status = CommonFnc::hexaToArray(getXMLElementValue(pRoot, "ad"), 
        static_cast<unsigned int>(pCaesarSettings->adLength), m_ad);
    if (status != STAT_OK) return status;
    status = CommonFnc::hexaToArray(getXMLElementValue(pRoot, "smn"), 
        static_cast<unsigned int>(pCaesarSettings->smnLength), m_smn);
    if (status != STAT_OK) return status;
    status = CommonFnc::hexaToArray(getXMLElementValue(pRoot, "pmn"), 
        static_cast<unsigned int>(pCaesarSettings->pmnLength), m_pmn);
    if (status != STAT_OK) return status;

    m_setup = true;
    return status;
}
