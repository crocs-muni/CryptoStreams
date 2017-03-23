#include "CaesarProject.h"

CAESAR_SETTINGS* pCaesarSettings = NULL;

CaesarProject::CaesarProject()
    : IProject(PROJECT_CAESAR), m_encryptor(NULL), m_ciphertext(NULL), m_realCiphertextLength(0) { }

CaesarProject::~CaesarProject() {
    if (m_encryptor != NULL) { delete m_encryptor; m_encryptor = NULL; }
    if (m_ciphertext != NULL) { delete[] m_ciphertext; m_ciphertext = NULL; }
}

string CaesarProject::shortDescription() const {
    return "CAESAR -- competition for authenticated encryption";
}

string CaesarProject::testingConfiguration() {
    string config =
            "<CAESAR>"
            "    <USAGE_TYPE>301</USAGE_TYPE>"
            "    <ALGORITHM>1</ALGORITHM>"
            "    <LIMIT_NUM_OF_ROUNDS>0</LIMIT_NUM_OF_ROUNDS>"
            "    <ALGORITHM_ROUNDS>3</ALGORITHM_ROUNDS>"
            "    <PLAINTEXT_LENGTH>16</PLAINTEXT_LENGTH>"
            "    <AD_LENGTH>2</AD_LENGTH>"
            "    <PLAINTEXT_TYPE>2</PLAINTEXT_TYPE>"
            "    <KEY_TYPE>1</KEY_TYPE>"
            "    <AD_TYPE>0</AD_TYPE>"
            "    <SMN_TYPE>0</SMN_TYPE>"
            "    <PMN_TYPE>0</PMN_TYPE>"
            "    <GENERATE_STREAM>0</GENERATE_STREAM>"
            "    <STREAM_SIZE>5242880</STREAM_SIZE>"
            "</CAESAR>";
    return config;
}

int CaesarProject::loadProjectConfiguration(TiXmlNode* pRoot) {
    m_caesarSettings.usageType = atoi(getXMLElementValue(pRoot,"CAESAR/USAGE_TYPE").c_str());
    m_caesarSettings.algorithm = atoi(getXMLElementValue(pRoot,"CAESAR/ALGORITHM").c_str());
    m_caesarSettings.limitAlgRounds = (atoi(getXMLElementValue(pRoot,"CAESAR/LIMIT_NUM_OF_ROUNDS").c_str())) ? true : false;
    m_caesarSettings.algorithmRoundsCount = atoi(getXMLElementValue(pRoot,"CAESAR/ALGORITHM_ROUNDS").c_str());
    istringstream(getXMLElementValue(pRoot,"CAESAR/PLAINTEXT_LENGTH")) >> m_caesarSettings.plaintextLength;
    istringstream(getXMLElementValue(pRoot,"CAESAR/AD_LENGTH")) >> m_caesarSettings.adLength;
    m_caesarSettings.plaintextType = atoi(getXMLElementValue(pRoot,"CAESAR/PLAINTEXT_TYPE").c_str());
    m_caesarSettings.keyType = atoi(getXMLElementValue(pRoot,"CAESAR/KEY_TYPE").c_str());
    m_caesarSettings.adType = atoi(getXMLElementValue(pRoot,"CAESAR/AD_TYPE").c_str());
    m_caesarSettings.smnType = atoi(getXMLElementValue(pRoot,"CAESAR/SMN_TYPE").c_str());
    m_caesarSettings.pmnType = atoi(getXMLElementValue(pRoot,"CAESAR/PMN_TYPE").c_str());
    m_caesarSettings.generateStream = (atoi(getXMLElementValue(pRoot,"CAESAR/GENERATE_STREAM").c_str())) ? true : false;
    istringstream(getXMLElementValue(pRoot,"CAESAR/STREAM_SIZE")) >> m_caesarSettings.streamSize;
    pCaesarSettings = &m_caesarSettings;

    // adjust number of rounds if necessary
    if (!m_caesarSettings.limitAlgRounds) { m_caesarSettings.algorithmRoundsCount = -1; }

    // bind project settings into global settings
    pGlobals->settings->project = (void*) pCaesarSettings;

    // TODO configuration checks

    return STAT_OK;
}

int CaesarProject::initializeProject() {
    // allocate encryptor
    m_encryptor = new Encryptor;
    // allocate ciphertext buffer
    m_ciphertext = new bits_t[pCaesarSettings->ciphertextLength];

    // TODO create headers for human readable test vector file

    return STAT_OK;
}

int CaesarProject::initializeProjectState() {
    int status = m_encryptor->setup();
    if (status == STAT_OK) {
        mainLogger.out(LOGGER_INFO) << "Cipher initialized (" << m_encryptor->shortDescription() << ")." << endl;
    } else {
        mainLogger.out(LOGGER_ERROR) << "Cipher could not be initialized (" << statusToString(status) << ")." << endl;
    }
    return status;
}

int CaesarProject::saveProjectState(TiXmlNode* pRoot) const {
    int status = STAT_OK;
    TiXmlElement* pRoot2 = pRoot->ToElement();
    pRoot2->SetAttribute("loadable",1);
    TiXmlElement* pElem = new TiXmlElement("encryptor");
    status = m_encryptor->saveState(pElem);
    pRoot2->LinkEndChild(pElem);
    return status;
}

int CaesarProject::loadProjectState(TiXmlNode* pRoot) {
    return m_encryptor->loadState(getXMLElement(pRoot, "encryptor"));
}

int CaesarProject::createTestVectorFilesHeaders() const {
    if (pGlobals->settings->outputs.saveTestVectors) {
        ofstream tvFile;
        tvFile.open(FILE_TEST_VECTORS, ios_base::trunc | ios_base::binary);
        if (!tvFile.is_open()) {
            mainLogger.out(LOGGER_ERROR) << "Cannot write file for test vectors (" << FILE_TEST_VECTORS << ")." << endl;
            return STAT_FILE_WRITE_FAIL;
        }
        tvFile << dec << left;
        tvFile << pCaesarSettings->algorithm << " \t\t(algorithm:" << m_encryptor->shortDescription() << ")" << endl;
        tvFile << pCaesarSettings->limitAlgRounds << " \t\t(limit algorithm rounds?)" << endl;
        tvFile << pCaesarSettings->algorithmRoundsCount << " \t\t(algorithm rounds - if limited)" << endl;
        tvFile << pCaesarSettings->plaintextLength << " \t\t(plaintext length)" << endl;
        tvFile << pCaesarSettings->adLength << " \t\t(associated data length)" << endl;
        tvFile << pCaesarSettings->plaintextType << " \t\t(plaintext type)" << endl;
        tvFile << pCaesarSettings->keyType << " \t\t(key type)" << endl;
        tvFile << pCaesarSettings->adType << " \t\t(associated data type)" << endl;
        tvFile << pCaesarSettings->smnType << " \t\t(secret message number type)" << endl;
        tvFile << pCaesarSettings->pmnType << " \t\t(public message number type)" << endl;
        tvFile.close();
    }

    return STAT_OK;
}

int CaesarProject::generateTestVectors() {
    int status = STAT_OK;

    // if set so, do not generate test vectors but generate data stream to cout
    if (pCaesarSettings->generateStream) {
        status = generateCipherDataStream();
        if (status != STAT_OK) {
            return status;
        } else {
            return STAT_INTENTIONAL_EXIT;
        }
    }

    // generate cipher stream
    for (int vector = 0; vector < pGlobals->settings->testVectors.setSize/2; vector++) {
        status = prepareSingleTestVector(pGlobals->testVectors.inputs[vector], pGlobals->testVectors.outputs[vector]);
        if (status != STAT_OK) { break; }
    }
    if (status != STAT_OK) {
        mainLogger.out(LOGGER_ERROR) << "Cipher data generation failed." << endl;
        return status;
    }
    // generate random vectors
    for (int vector = pGlobals->settings->testVectors.setSize/2; vector < pGlobals->settings->testVectors.setSize; vector++) {
        // random stream
        for (int byte = 0; byte < pGlobals->settings->testVectors.inputLength; byte++) {
            rndGen->getRandomFromInterval(255, pGlobals->testVectors.inputs[vector] + byte);
        }
        // 0xff to denote random stream
        for (int byte = 0; byte < pGlobals->settings->testVectors.outputLength; byte++) {
            pGlobals->testVectors.outputs[vector][byte] = UCHAR_MAX;
        }
    }

    return status;
}

int CaesarProject::prepareSingleTestVector(unsigned char* tvInputs, unsigned char* tvOutputs) {
    int status = STAT_OK;
    switch (pCaesarSettings->usageType) {
    case CAESAR_DISTINGUISHER_TAG:
        // ciphertext stream
        status = m_encryptor->encrypt(m_ciphertext, &m_realCiphertextLength);
        if (status != STAT_OK) { return status; }
        // copy authentication tag
        memcpy(tvInputs, m_ciphertext+m_caesarSettings.plaintextLength,
                min(m_realCiphertextLength-m_caesarSettings.plaintextLength, (length_t)pGlobals->settings->testVectors.inputLength));
        // fill with zeroes
        if ((int) (m_realCiphertextLength-m_caesarSettings.plaintextLength) < pGlobals->settings->testVectors.inputLength) {
            mainLogger.out(LOGGER_WARNING) << "Authentication tag shorter than test vector input -- padded with zeroes." << endl;
            for (int byte = static_cast<int>(m_caesarSettings.plaintextLength); byte < pGlobals->settings->testVectors.inputLength; byte++) {
                tvInputs[byte] = 0;
            }
        }
        status = m_encryptor->update();
        if (status != STAT_OK) { return status; }
        // 0x00 to denote ciphertext stream
        for (int byte = 0; byte < pGlobals->settings->testVectors.outputLength; byte++) {
            tvOutputs[byte] = 0;
        }
        break;
    case CAESAR_DISTINGUISHER_CIPHERTEXT:
        // ciphertext stream
        status = m_encryptor->encrypt(m_ciphertext, &m_realCiphertextLength);
        if (status != STAT_OK) { return status; }
        // copy ciphertext
        memcpy(tvInputs, m_ciphertext,
                min(m_realCiphertextLength-m_caesarSettings.plaintextLength, (length_t)pGlobals->settings->testVectors.inputLength));
        // fill with zeroes
        if ((int) (m_realCiphertextLength-m_caesarSettings.plaintextLength) < pGlobals->settings->testVectors.inputLength) {
            mainLogger.out(LOGGER_WARNING) << "Authentication tag shorter than test vector input -- padded with zeroes." << endl;
            for (int byte = static_cast<int>(m_caesarSettings.plaintextLength); byte < pGlobals->settings->testVectors.inputLength; byte++) {
                tvInputs[byte] = 0;
            }
        }
        status = m_encryptor->update();
        if (status != STAT_OK) { return status; }
        // 0x00 to denote ciphertext stream
        for (int byte = 0; byte < pGlobals->settings->testVectors.outputLength; byte++) {
            tvOutputs[byte] = 0;
        }
        break;
    default:
        mainLogger.out(LOGGER_ERROR) << "Unknown usage type (" << pCaesarSettings->usageType << ") in " << shortDescription() << endl;
        return STAT_INVALID_ARGUMETS;
        break;
    }
    return status;
}

int CaesarProject::generateCipherDataStream() {
    int status = STAT_OK;

    unsigned char* tvOutputs = new unsigned char[pGlobals->settings->testVectors.outputLength];
    memset(tvOutputs,0,pGlobals->settings->testVectors.outputLength);
    unsigned char* tvInputs = new unsigned char[pGlobals->settings->testVectors.inputLength];
    memset(tvInputs,0,pGlobals->settings->testVectors.inputLength);

    string streamFilename = CAESAR_FILE_STREAM;
    mainLogger.out(LOGGER_INFO) << "Generating cipher data stream for " << m_encryptor->shortDescription() << "." << endl;


    ostream* vectorStream = NULL;
    if (pCaesarSettings->streamSize == 0) {
        vectorStream = &cout;
        mainLogger.out(LOGGER_INFO) << "Sneding binary stream to standard output." << endl;
    } else {
        vectorStream = new ofstream(streamFilename, ios_base::binary | ios_base::trunc);
        mainLogger.out(LOGGER_INFO) << "Saving binary stream to file '" << streamFilename << "'." << endl;
    }
    unsigned long alreadyGenerated = 0;
    while (pCaesarSettings->streamSize == 0 ? true : alreadyGenerated <= pCaesarSettings->streamSize) {
        for (int testVectorNumber = 0; testVectorNumber < pGlobals->settings->testVectors.setSize; testVectorNumber++) {
            if (status != STAT_OK) break;

            // get test vector
            status = prepareSingleTestVector(tvInputs,tvOutputs);

            for (int index = 0; index < pGlobals->settings->testVectors.inputLength; index++) {
                (*vectorStream) << tvInputs[index];
            }
            alreadyGenerated += pGlobals->settings->testVectors.inputLength;
        }
    }

    if (pCaesarSettings->streamSize != 0) {
        delete vectorStream;
        vectorStream = NULL;
    }
    if (status == STAT_OK) {
        mainLogger.out(LOGGER_INFO) << "Cipher data generation successful (" << alreadyGenerated << " bytes)." << endl;
    } else {
        mainLogger.out(LOGGER_ERROR) << "Cipher data generation failed (" << alreadyGenerated << " bytes generated)." << endl;
    }

    delete[] tvInputs;
    delete[] tvOutputs;

    return status;
}
