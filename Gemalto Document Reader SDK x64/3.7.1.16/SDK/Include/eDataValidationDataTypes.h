#ifndef __EDATAVALIDATIONDATATYPES_H__
#define __EDATAVALIDATIONDATATYPES_H__

#ifndef MAX_PATH
#define MAX_PATH          260
#endif

#if !(defined(__linux__) && defined(__arm__))
#pragma pack(push, 1)
#endif

/// Defines eData validation result codes.
typedef enum eDataValidationResultCode
{
	EDV_SUCCESS = 0,
	EDV_VALID,
	EDV_VALID_WITH_REVOKED_CERT,
	EDV_INVALID,
	EDV_DSC_NOT_FOUND,
	EDV_CSC_NOT_FOUND,
	EDV_VALIDATION_ERROR,
	EDV_INVALID_PARAMS,
	EDV_ERROR_DECODING_SOD,
	EDV_ERROR_DECODING_COM,
	EDV_NOT_INITIALISED,
	EDV_SOD_NOT_SET,
	EDV_SOD_HASH_NOT_PRESENT,
	EDV_COM_NOT_SET,
	EDV_VALIDATION_EXCEPTION,
	EDV_DGC_INVALID_CWT_VERSION,
	EDV_DGC_ERROR_LOADING_DGC,
	EDV_DGC_NOT_LOADED,
	EDV_DGC_PARSE_ERROR,
	EDV_DGC_WARNING_MULTIPLE_VACCINATION_ENTRIES,
	EDV_DGC_WARNING_MULTIPLE_TEST_ENTRIES,
	EDV_DGC_WARNING_MULTIPLE_RECOVERY_ENTRIES,
	EDV_DGC_WARNING_MULTIPLE_VTR_ENTRIES,
	EDV_DGC_RUNTIME_EXCEPTION,
	EDV_DGC_NOT_INITIALISED
} eDataValidationResultCode;

/// Defines the supported external certificate sources supported by the \sdk for RF read
/// operations.
///
/// \ingroup RfidModule
typedef enum EXTERNAL_CERT_MODE
{
	/// Retrieve a certificate from a number of certificate files contained in a local
	/// directory.
	///
	/// \note This mode can be used for the following RF operations:
	///		- Validating the signature in a Document Security Object using a public key
	///		from a Document Signer Certificate (DSC).
	///		- Validating the signature of a DSC using the public key from an external
	///		Country Signer Certificate (CSC).
	///		- \eac Terminal Authentication
	///
	/// \remarks The folder where the certificate files are located can be set via the 
	/// #RFProcessSettings::puCertsDir setting. The #RFProcessSettings structure also
	/// provides settings for including sub-directories and specifying what certificate file
	/// extensions to search for.
	///
	/// \sa #RFProcessSettings
	ECM_CERT_FILE_STORE = 1,

	/// Retrieve a certificate by having the \sdk call back to the hosting application
	/// using the #MMMReaderCertificateCallback. In this scenario, the hosting application
	/// is responsible for providing the correct certificate to use for the operation.
	///
	/// \note This mode can be used for the following RF operations:
	///		- Validating the signature in a Document Security Object using a public key
	///		from a Document Signer Certificate (DSC).
	///		- Validating the signature of a DSC using the public key from an external
	///		Country Signer Certificate (CSC).
	///		- \eac Terminal Authentication
	///
	/// \sa #MMMReaderCertificateCallback
	ECM_CERT_CALLBACK = 2,

	/// \eac Request that a signature is generated over some data provided by the \sdk to verify
	/// Terminal Authentication.
	///
	/// \note This mode can be used for the following RF operations:
	///		- \eac Terminal Authentication
	///
	/// \remarks This method is provided for systems that do not allow private keys to be
	/// passed around in files or memory, e.g. using HSM or SAM implementations.
	///
	/// \remarks The \sdk will invoke a #MMMReaderSignRequestCallback, which requires the
	/// host application to generate a signature over a given buffer. The IS certificate will
	/// be included in the callback to provide the host application with all the necessary 
	/// parameters for algorithm and domain parameter selection.
	///
	/// \sa #MMMReaderSignRequestCallback
	ECM_SIGN_REQUEST = 3,

	/// \eac Request that Terminal Authentication be carried out by a remote server using the
	/// Inspection System Server. Contact \ssd for more details.    
	///
	/// \note This mode can be used for the following RF operations:
	///		- \eac Terminal Authentication
	ECM_USE_3M_ISS = 4

} EXTERNAL_CERT_MODE;

/// Defines Digital Green Certificate (DGC) / Digital COVID Signature (DCC) related settings.
typedef struct DGCSettings_s
{
	/// Specifies how to retrieve Document Signer Certificates (DSCs). See #EXTERNAL_CERT_MODE
	/// for an explanation of the available modes.
	EXTERNAL_CERT_MODE puDGCExternalDSCMode;

	/// Specifies how to retrieve Country Signer Certificates (CSCs). See #EXTERNAL_CERT_MODE
	/// for an explanation of the available modes.
	EXTERNAL_CERT_MODE puDGCExternalCSCMode;

	/// Specify how to retrieve Certificate Revocation Lists (CRLs) for Document Signer 
	/// Certificates (DSCs). See #EXTERNAL_CERT_MODE for an explanation of the available modes.
	///
	/// \note This setting is only relevant if #puCheckDSCRevocation is \c true.
	EXTERNAL_CERT_MODE puDGCDSC_CRLMode;

	/// Specify how to retrieve Certificate Revocation Lists (CRLs) for Country Signer 
	/// Certificates (DSCs). See #EXTERNAL_CERT_MODE for an explanation of the available modes.
	///
	/// \note This setting is only relevant if #puCheckCSCRevocation is \c true.
	EXTERNAL_CERT_MODE puDGCCSC_CRLMode;

	/// Provides a folder location on the host machine where one or more certificates are 
	/// stored.
	///
	/// \sa #EXTERNAL_CERT_MODE::ECM_CERT_FILE_STORE
	char puDGCCertsDir[MAX_PATH];

	/// Provides a list of certificate file extensions to search for when searching #puCertsDir
	/// for certificates.
	///
	/// \remarks This value allows only specified certificate types to be loaded.
	///
	/// \remarks The format of this value should be each file extension (minus the period
	/// character) separated by semi-colons, e.g. \c "cer;der;pem;crt;crl"
	///
	/// \sa #EXTERNAL_CERT_MODE::ECM_CERT_FILE_STORE
	char puDGCCertFileExtensions[256];

	/// Determines whether to check Document Signer Certificates (DSCs) for revocation against
	/// one or more Certificate Revocation Lists (CRLs).
	///
	/// \sa #puDSC_CRLMode
	bool puDGCCheckDSCRevocation;

	/// Determines whether to check Country Signer Certificates (DSCs) for revocation against
	/// one or more Certificate Revocation Lists (CRLs).
	///
	/// \sa #puCSC_CRLMode
	bool puDGCCheckCSCRevocation;

	/// Determines whether to validate the signature.
	bool puDGCValidateSignature;

	/// Determines whether to validate the Document Signer Certificate (DSC).
	bool puDGCValidateDocSignerCert;

} DGCSettings;

/// Defines eData validation settings.
typedef struct eDataValidationSettings
{
	/// The file path to certificates and/or CRLs (cer;der;pem;crt;crl;cvcert;pkcs8) to be loaded for ePassport validation, if desired.
	///
	/// \remarks This path is optional for certificates as they may be exlicitly passed to applicable methods.
	char certificatePath[260];

	/// Settings for EU Digital Green Certificates (DGC) / Digital COVID Certificates (DCC) used in eDV_DGC methods.
	/// 
	DGCSettings dgcSettings;

} eDataValidationSettings;

#define EDATA_DTC_DATA_NUM_DATAGROUPS 21

/// Defines data from a DTC (Digital Travel Credential).
typedef struct eDV_DTCData
{
	/// EF.SOD file from DTC.
	unsigned char* efSOD;

	/// Length of EF.SOD.
	int efSODLength;

	/// DataGroup files from DTC.
	unsigned char* dataGroups[EDATA_DTC_DATA_NUM_DATAGROUPS];

	/// DataGroup lengths.
	int dataGroupLengths[EDATA_DTC_DATA_NUM_DATAGROUPS];

} eDV_DTCData;

//-------------------------------------------------------------------------------------------
// Health Certificate Claims
// Digital Green Certificate/Digital COVID Certificate
//-------------------------------------------------------------------------------------------

#define DGC_CWT_VERSION_LEN		10
#define DGC_CWT_KID_LEN			8
#define DGC_CWT_ISSUER_LEN		5
#define DGC_CWT_TIMESTAMP_LEN	30
#define DGC_CWT_HCERT_LEN		2000

#define DGC_HCERT_NAME_LEN	400
#define DGC_HCERT_DATE_LEN	15
#define DGC_HCERT_IDENTIFIER_LEN	100

#define DGC_V_GROUP_CODE_LEN	50

/// Defines a Vaccination Group in a DGC/DCC.
/// 
///<a href="https://ec.europa.eu/health/sites/default/files/ehealth/docs/covid-certificate_json_specification_en.pdf" target="_blank">DGC/DCC JSON Specification</a>)
typedef struct eDV_DGCVaccinationGroup
{
	/// Dose Number Ex. "1" (JSON field "v/dn")
	int DoseNumber;

	/// Total Number of Doses Ex. "3" (JSON field "v/sd")
	int TotalDoses;

	/// Disease or Agent Targeted Ex. "840539006" (JSON field "v/tg") (Maps to disease-agent-targeted.json)
	char TargetedDiseaseOrAgent[DGC_V_GROUP_CODE_LEN];

	/// Vaccine or Prophylaxis Ex. "1119349007" (JSON field "v/vp") (Maps to vaccine-prophylaxis.json)
	char VaccineOrProphylaxis[DGC_V_GROUP_CODE_LEN];

	/// Vaccine Product Ex. "EU/1/20/1528" (JSON field "v/mp") (Maps to vaccine-medicinal-product.json)
	char VaccineProduct[DGC_V_GROUP_CODE_LEN];

	/// Vaccine Markenting Authorization Holder or Manufacturer Ex. "ORG-10030215" (JSON field "v/ma") (Maps to vaccine-mah-manf.json)
	char VaccineManufacturerOrHolder[DGC_V_GROUP_CODE_LEN];

	/// Date of Vaccination Ex. "YYYY-MM-DD" (JSON field "v/dt")
	char DateOfVaccination[DGC_HCERT_DATE_LEN];

	/// Country or Event Organizer Administered Ex. "CZ" or "WHO" (JSON field "v/co") (Maps to country-2-codes.json)
	char CountryAdministered[DGC_V_GROUP_CODE_LEN];

	/// Health Certificate Issuer Ex. "Ministry of Health of the Czech Republic" (JSON field "v/is")
	char CertificateIssuer[DGC_HCERT_NAME_LEN];

	/// Unique Certificate Identifier Ex. "URN:UVCI:01:AT:10807843F94AEE0EE5093FBC254BD813#B" (JSON field "v/ci")
	char CertificateIdentifier[DGC_HCERT_IDENTIFIER_LEN];

} eDV_DGCVaccinationGroup;

/// Defines a Test Group in a DGC/DCC.
/// 
///<a href="https://ec.europa.eu/health/sites/default/files/ehealth/docs/covid-certificate_json_specification_en.pdf" target="_blank">DGC/DCC JSON Specification</a>)
typedef struct eDV_DGCTestGroup
{
	/// Disease or Agent Targeted Ex. "840539006" (JSON field "t/tg") (Maps to disease-agent-targeted.json)
	char TargetedDiseaseOrAgent[DGC_V_GROUP_CODE_LEN];

	/// Type of Test Ex. "LP6464-4" (JSON field "t/tt") (Maps to test-type.json)
	char TestType[DGC_V_GROUP_CODE_LEN];

	/// Test Name Ex. "ELITechGroup, SARS-CoV-2 ELITe MGB® Kit"" (JSON field "t/nm")
	char TestName[DGC_HCERT_NAME_LEN];

	/// Test Device Identifier Ex. "344" (JSON field "t/ma")
	char TestDeviceIdentifier[DGC_V_GROUP_CODE_LEN];

	/// Date and Time of Sample Collection Ex. "2021-08-20T12:03:12+02:00" (JSON field "t/sc")
	char SampleCollectionTime[DGC_CWT_TIMESTAMP_LEN];

	/// Test Result Ex. "260415000" (JSON field "t/tr") (Maps to test-result.json)
	char TestResult[DGC_V_GROUP_CODE_LEN];

	/// Testing Centre or Facility Ex. "Test centre west region 245" (JSON field "t/tc")
	char TestingFacility[DGC_HCERT_NAME_LEN];

	/// Country or Event Organizer Administered Ex. "CZ" or "WHO" (JSON field "t/co")
	char CountryAdministered[DGC_V_GROUP_CODE_LEN];

	/// Health Certificate Issuer Ex. "Ministry of Health of the Czech Republic" (JSON field "t/is")
	char CertificateIssuer[DGC_HCERT_NAME_LEN];

	/// Unique Certificate Identifier Ex. "URN:UVCI:01:AT:10807843F94AEE0EE5093FBC254BD813#B" (JSON field "t/ci")
	char CertificateIdentifier[DGC_HCERT_IDENTIFIER_LEN];

} eDV_DGCTestGroup;

/// Defines a Recovery Group in a DGC/DCC.
/// 
///<a href="https://ec.europa.eu/health/sites/default/files/ehealth/docs/covid-certificate_json_specification_en.pdf" target="_blank">DGC/DCC JSON Specification</a>)
typedef struct eDV_DGCRecoveryGroup
{
	/// Disease or Agent Recovered From Ex. "840539006" (JSON field "r/tg") (Maps to disease-agent-targeted.json)
	char RecoveredFromDiseaseOrAgent[DGC_V_GROUP_CODE_LEN];

	/// Date of Holder's First Positive Test Ex. "YYYY-MM-DD" (JSON field "r/fr")
	char DateOfFirstPositiveTest[DGC_HCERT_DATE_LEN];

	/// Country or Event Organizer Positive Test Administered Ex. "CZ" or "WHO" (JSON field "r/co")
	char CountryPositiveTestAdministered[DGC_V_GROUP_CODE_LEN];

	/// Health Certificate Issuer Ex. "Ministry of Health of the Czech Republic" (JSON field "r/is")
	char CertificateIssuer[DGC_HCERT_NAME_LEN];

	/// Date Recovery Certificate Valid From Ex. "YYYY-MM-DD" (JSON field "r/df")
	char DateCertificateValidFrom[DGC_HCERT_DATE_LEN];

	/// Date Recovery Certificate Valid Until Test Ex. "YYYY-MM-DD" (JSON field "r/du")
	char DateCertificateValidUntil[DGC_HCERT_DATE_LEN];

	/// Unique Certificate Identifier Ex. "URN:UVCI:01:AT:10807843F94AEE0EE5093FBC254BD813#B" (JSON field "r/ci")
	char CertificateIdentifier[DGC_HCERT_IDENTIFIER_LEN];

} eDV_DGCRecoveryGroup;

/// Defines the CWT (CBOR Web Token) format of a Health Certificate CWT Claim,
/// which contains a Health Certificate, such as the EU Digital Green Certificate (DGC)/
/// Digital COVID Certificate (DCC)
/// See <a href="https://ec.europa.eu/health/sites/default/files/ehealth/docs/digital-green-certificates_v3_en.pdf" target="_blank">DGC/DCC Specification</a>
typedef struct eDV_HealthCertificateClaim
{
	/// CWT Signing Algorithm ID
	int Algorithm;

	int KeyIdentifierLen;

	/// Certificate Version "HC1", "HC2", etc.
	char Version[DGC_CWT_VERSION_LEN];

	/// CWT Signing Key ID (First 8 bytes of SHA256 hash of DSC (Doc Signer Certificate))
	unsigned char KeyIdentifier[DGC_CWT_KID_LEN];

	/// Issuer
	char Issuer[DGC_CWT_ISSUER_LEN];

	/// Issue Time
	char IssueTime[DGC_CWT_TIMESTAMP_LEN];

	/// Expiration Time
	char ExpiryTime[DGC_CWT_TIMESTAMP_LEN];

	/// Health Certificate, such as DGC/DCC, in JSON format.
	/// Ex. <a href="https://ec.europa.eu/health/sites/default/files/ehealth/docs/covid-certificate_json_specification_en.pdf" target="_blank">DGC/DCC JSON Specification</a>)
	char HealthCertificate[DGC_CWT_HCERT_LEN];

	/// True if the certificate is valid based on Issue and Expiry time.
	bool DateRangeValid;

} eDV_HealthCertificateClaim;

/// Defines the data elements of the EU Digital Green Certificate (DGC)/
/// Digital COVID Certificate (DCC) Health Certificate in a HealthCertificateClaim.
typedef struct eDV_DigitalGreenCertificateData
{
	/// The Health Certificate Claim, which is the parent CWT container of the DGC/DCC,
	/// and contains the unparsed HealthCertificate JSON data and other claim data.
	eDV_HealthCertificateClaim CertificateClaim;

	/// Vaccination Group (JSON field "v")
	eDV_DGCVaccinationGroup VaccinationGroup;

	/// Test Group (JSON field "t")
	eDV_DGCTestGroup TestGroup;

	/// Recovery Group (JSON field "r")
	eDV_DGCRecoveryGroup RecoveryGroup;

	/// Schema Version Ex. "1.3.0" (JSON field "ver")
	char Version[DGC_CWT_VERSION_LEN];

	/// Surnames (JSON field "nam/fn")
	char Surnames[DGC_HCERT_NAME_LEN];

	/// Surnames in ICAO MRZ format (JSON field "nam/fnt")
	char StandardizedSurnames[DGC_HCERT_NAME_LEN];

	/// Fornames (JSON field "nam/gn")
	char Fornames[DGC_HCERT_NAME_LEN];

	/// Standardized Fornames (JSON field "nam/gnt")
	char StandardizedFornames[DGC_HCERT_NAME_LEN];

	/// Date of Birth (JSON field "dob") (YYYY-MM-DD, YYYY-MM or YYYY)
	char DateOfBirth[DGC_HCERT_DATE_LEN];

} eDV_DigitalGreenCertificateData;

#if !(defined(__linux__) && defined(__arm__))
#pragma pack(pop)
#endif


#endif
