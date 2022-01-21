#ifndef _RTQADataTypes_H_
#define _RTQADataTypes_H_

#define MAX_CODELINE_LENGTH 200	
#define MAX_CODELINE_FIELD_LENGTH	40
#define MAX_OPTIONAL_DATA_LENGTH	40
#define MAX_DATA_FIELD_LENGTH	1024
#define MAX_CHECKDIGIT_LENGTH 30
#define MAX_CHECKDIGITDATA_COUNT 5

typedef enum RTQA_ParseCodelineFlag
{
	/// No special parsing required.
	RTQA_MPCF_NONE				= 0x00,
	
	/// Set the #MMMReaderCodelineData::Data member with the value that 
	/// MMMReader_ParseCodeline() has extracted.
	RTQA_MPCF_SET_DATA_FIELD		= 0x01,
	
	/// Set the #MMMReaderCodelineData::Line1, #MMMReaderCodelineData::Line2 and 
	/// #MMMReaderCodelineData::Line3 members with the values that MMMReader_ParseCodeline()
	/// has extracted.
	RTQA_MPCF_SET_LINE_FIELDS	= 0x02,
	
	/// Carry out all of the above operations.
	RTQA_MPCF_ALL = RTQA_MPCF_SET_DATA_FIELD | RTQA_MPCF_SET_LINE_FIELDS

} RTQA_ParseCodelineFlag;

typedef struct RTQA_ReaderDate
{
	/// Provides the day part of a date. This will be between 1 - 31.
	int Day;
	
	/// Provides the month part of a date. This will be between 1 - 12.
	int Month;
	
	/// Provides the year part of a date.
	///
	/// \attention Depending on the input source, the #Year may be either a 2-digit year or
	/// a 4-digit year. If it is a 2-digit year, consideration must be taken when interpreting
	/// the value.
	int Year;

} RTQA_ReaderDate;

/// Defines the types of check digit found in an ICAO codeline
typedef enum RTQA_ReaderCheckDigitType
{
	/// Document number check digit
	RTQA_CDT_DocID,

	/// Date of birth check digit
	RTQA_CDT_DOB,

	/// Expiry date check digit
	RTQA_CDT_Expiry,

	/// Optional data check digit
	RTQA_CDT_OptionalData,

	/// Overall check digit for the whole codeline
	RTQA_CDT_Overall

} RTQA_ReaderCheckDigitType;

/// Defines the possible result values of a check digit validation
typedef enum RTQA_ReaderCheckDigitResult
{
	/// Check digit is invalid
	RTQA_CDR_Invalid,

	/// Check digit is valid
	RTQA_CDR_Valid,

	/// Check digit is not valid according to ICAO rules, but this maybe expected
	RTQA_CDR_Warning,

	/// Check digit validation has not been carried out
	RTQA_CDR_NotValidated

} RTQA_ReaderCheckDigitResult;

/// Defines data about a check digit from an OCR ICAO codeline.
typedef struct RTQA_ReaderCodelineCheckDigitData
{
	/// The codeline check digit type.
	RTQA_ReaderCheckDigitType puCheckDigitType;

	/// The codeline number on which the check digit is located.
	///
	/// \remarks This is a 1 based index (1, 2, or 3)
	int puCodelineNumber;

	/// The character position on the line of the codeline on which the check digit
	/// is found.
	///
	/// \remarks This is a 0 based index
	int puCodelinePos;

	/// The expected check digit value, based on the rest of the codeline characters
	/// read.
	char puValueExpected;

	/// The value of the check digit read from the codeline.
	char puValueRead;

	/// The result
	RTQA_ReaderCheckDigitResult puResult;

} RTQA_ReaderCodelineCheckDigitData;

typedef enum RTQA_DataType
{
	QA_DT_TRAVEL_DOC_TYPE_COUNT,		// Number of Travel Documents					[int]
	QA_DT_TRAVEL_DOC_TYPE,				// Travel Document ID and Name					[RTQA_TravelDocType]

	QA_DT_IMAGE_IR,						// The IR image 								[Array of unsigned char]
	QA_DT_IMAGE_VIS,					// The visible image							[Array of unsigned char]
	QA_DT_IMAGE_UV,						// The UV image.								[Array of unsigned char]
	QA_DT_IMAGE_COAX,					// The Coax image.								[Array of unsigned char]
	QA_DT_IMAGE_COAXIR,					// The Coax IR image							[Array of unsigned char]

	QA_DT_MRZ_CODELINE_COUNT,			// Number of MRZ codelines						[int]
	QA_DT_MRZ_CODELINE_CHAR_COUNT,		// MRZ codeline character count					[RTQA_MRZCodelineData containing int]
	QA_DT_MRZ_CODELINE_TOP,				// MRZ codeline top position					[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_BOTTOM,			// MRZ codeline bottom position					[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_LEFT,			// MRZ codeline left position					[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_RIGHT,			// MRZ codeline right position					[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_SKEW,			// MRZ codeline skew angle						[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_OCR_RESULTS,		// MRZ codeline decoded chars					[RTQA_MRZCodelineData containing char array]
	QA_DT_MRZ_CODELINE_OCR_SCORES,		// MRZ codeline scores for decoded chars		[RTQA_MRZCodelineData containing int array]
	QA_DT_MRZ_CODELINE_AVE_STROKE_WIDTH,// MRZ codeline average stroke width of chars	[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_BAD_CHAR_COUNT,	// MRZ codeline number of chars not decoded		[RTQA_MRZCodelineData containing int]
	QA_DT_MRZ_CODELINE_BROKEN_CHAR_COUNT,// 		[RTQA_MRZCodelineData containing int]
	QA_DT_MRZ_CODELINE_CHAR_HEIGHT,		// MRZ codeline height							[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_WIDTH,			// MRZ codeline width							[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_MIDDLE,			// MRZ codeline vertical mid-point position		[RTQA_MRZCodelineData containing float]
	QA_DT_MRZ_CODELINE_GAP_BELOW,		// MRZ codeline gap to codeline below/edge of doc [RTQA_MRZCodelineData containing float]

	QA_DT_MRZ_Y_OFFSET,					//												[float]
	QA_DT_MRZ_PIXEL_SIZE,				//												[float]
	QA_DT_MRZ_NUM_OF_SPOTS,				// Number of spots detected in the MRZ			[int]
	QA_DT_MRZ_CHECKDIGITS_VALIDATE,		// Whether the checkdigits in the MRZ are valid	[bool]

	QA_DT_RFID_EF_COM_FILE,				// Raw bytes data-file for EF.COM				[char*]
	QA_DT_RFID_EF_SOD_FILE,				// Raw bytes data-file for EF.SOD				[char*]
	QA_DT_RFID_DG1_FILE,				// Raw bytes data-file for DG1					[char*]
	QA_DT_RFID_DG2_FILE,				// Raw bytes data-file for DG2					[char*]
	QA_DT_RFID_DG3_FILE,				// Raw bytes data-file for DG3					[char*]
	QA_DT_RFID_DG4_FILE,				// Raw bytes data-file for DG4					[char*]
	QA_DT_RFID_DG5_FILE,				// Raw bytes data-file for DG5					[char*]
	QA_DT_RFID_DG6_FILE,				// Raw bytes data-file for DG6					[char*]
	QA_DT_RFID_DG7_FILE,				// Raw bytes data-file for DG7					[char*]
	QA_DT_RFID_DG8_FILE,				// Raw bytes data-file for DG8					[char*]
	QA_DT_RFID_DG9_FILE,				// Raw bytes data-file for DG9					[char*]
	QA_DT_RFID_DG10_FILE,				// Raw bytes data-file for DG10					[char*]
	QA_DT_RFID_DG11_FILE,				// Raw bytes data-file for DG11					[char*]
	QA_DT_RFID_DG12_FILE,				// Raw bytes data-file for DG12					[char*]
	QA_DT_RFID_DG13_FILE,				// Raw bytes data-file for DG13					[char*]
	QA_DT_RFID_DG14_FILE,				// Raw bytes data-file for DG14					[char*]
	QA_DT_RFID_DG15_FILE,				// Raw bytes data-file for DG15					[char*]
	QA_DT_RFID_DG16_FILE,				// Raw bytes data-file for DG16					[char*]
	
	QA_DT_RFID_DG1_VALIDATE,			// Indicates whether DG1 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG2_VALIDATE,			// Indicates whether DG2 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG3_VALIDATE,			// Indicates whether DG3 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG4_VALIDATE,			// Indicates whether DG4 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG5_VALIDATE,			// Indicates whether DG5 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG6_VALIDATE,			// Indicates whether DG6 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG7_VALIDATE,			// Indicates whether DG7 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG8_VALIDATE,			// Indicates whether DG8 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG9_VALIDATE,			// Indicates whether DG9 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG10_VALIDATE,			// Indicates whether DG10 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG11_VALIDATE,			// Indicates whether DG11 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG12_VALIDATE,			// Indicates whether DG12 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG13_VALIDATE,			// Indicates whether DG13 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG14_VALIDATE,			// Indicates whether DG14 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG15_VALIDATE,			// Indicates whether DG15 hash is valid			[int 1=pass, 0=fail]
	QA_DT_RFID_DG16_VALIDATE,			// Indicates whether DG16 hash is valid			[int 1=pass, 0=fail]
	
	QA_DT_RFID_SIGNEDATTRS_VALIDATE,	// Indicates whether the signed attributes are valid [int 1=pass, 0=fail]
	QA_DT_RFID_SIGNATURE_VALIDATE,		// Indicates whether the signature is valid		[int 1=pass, 0=fail]
	QA_DT_RFID_BAC_STATUS,				// Indicates whether BAC is active				[int 1=pass, 0=fail]

	QA_DT_RFID_AIRBAUD,					// The aid baud rate used						[char*]
	QA_DT_RFID_CHIPID,					// The chip id									[char*]

	QA_DT_RFID_DECODED_DG1_CODELINE,	// MRZ data, extracted from DG1					[char*]
	QA_DT_RFID_DECODED_DG2_PHOTO,		// Photo data, extracted from DG2				[unsigned char*]
	
	QA_DT_CODELINE_TEST_RESULT,			// QA Result for a codeline test				[RTQA_CodelineTestResult]
	QA_DT_TEST_RESULT,					// QA Result for a general test					[RTQA_TestResult]
	QA_DT_CODELINE_TEST_RESULT_COUNT,	// Number of QA codeline results 				[int]
	QA_DT_TEST_RESULT_COUNT,			// Number of QA results							[int]

	QA_DT_RFID_ACTIVE_AUTHENTICATION,
	QA_DT_RFID_VALIDATE_DOC_SIGNER_CERT,
	QA_DT_RFID_CHIP_AUTH_STATUS,
	QA_DT_RFID_TERMINAL_AUTH_STATUS,
	QA_DT_RFID_EF_CVCA_FILE,
} RTQA_DataType;

typedef enum RTQA_EventType
{
	QA_ET_INITIALISE_LOAD_DOCS_CONFIG,		//Fired before Document Type XML file is loaded
	QA_ET_INITIALISE_LOAD_READER_CONFIG,	//Fired before ini files are loaded from Reader
	QA_ET_INITIALISE_SETUP_READER,			//Fired before the Reader is initialised
	QA_ET_INITIALISE_COMPLETE,				//Fired when initialisation is complete
	QA_ET_DOC_TYPES_LIST_START,				//Fired before DocumentType list is sent
	QA_ET_DOC_TYPES_LIST_END,				//Fired when DocumentType list has finished being sent
	QA_ET_READY_FOR_COMMAND,				//Fired when reader is awaiting a command
	QA_ET_DOC_ON_WINDOW,					//Fired when a document is detected on the Reader window
	QA_ET_DOC_OFF_WINDOW,					//Fired when it is detected a documnet has been removed from the Reader window
	QA_ET_DOC_READ_START,					//Fired when we start capturing a document data
	QA_ET_DOC_READ_END,						//Fired when we have finished capturing document data
	QA_ET_DOC_DATA_RESULTS_START,			//Fired when we start document QA tests
	QA_ET_DOC_DATA_RESULTS_END,				//Fired when we finish document QA tests
	QA_ET_ERROR_RECOVERY_LAUNCHED			//Fired when a serious error has occured and the reader is being re-started

} RTQA_EventType;

typedef enum RTQA_TestID
{
	RTQA_TID_IMAGE_VISIBLE = 000,
	RTQA_TID_IMAGE_IR = 001,
	RTQA_TID_IMAGE_UV = 002,
	RTQA_TID_IMAGE_COAX = 003,
	RTQA_TID_IMAGE_COAXIR = 004,

	RTQA_TID_MRZ_NUM_OF_CHARS = 100,
	RTQA_TID_MRZ_LEFT_MARGIN = 101,
	RTQA_TID_MRZ_RIGHT_MARGIN = 102,
	RTQA_TID_MRZ_POSITION_TOP = 103,
	RTQA_TID_MRZ_POSITION_BOTTOM = 104,
	RTQA_TID_MRZ_SKEW = 105,
	RTQA_TID_MRZ_SPOT_THRESHOLD = 106,
	RTQA_TID_MRZ_BAD_CHARS = 107,
	RTQA_TID_MRZ_BROKEN_CHARS = 108,
	RTQA_TID_MRZ_AVE_STROKE_WIDTH = 109,
	RTQA_TID_MRZ_VALID_CHECK_DIGITS = 110,
	RTQA_TID_MRZ_CHAR_HEIGHT = 111,
	RTQA_TID_MRZ_CODELINE_WIDTH = 112,
	RTQA_TID_MRZ_POSITION_MIDDLE = 113,
	RTQA_TID_MRZ_GAP_BELOW = 114,

	RTQA_TID_MRZ_CODELINE_VALID = 199,

	RTQA_TID_RFID_BAC = 200,
	RTQA_TID_RFID_COM_FILE = 201,
	RTQA_TID_RFID_SOD_FILE = 202,
	RTQA_TID_RFID_DG1_FILE = 203,
	RTQA_TID_RFID_DG2_FILE = 204,
	RTQA_TID_RFID_DG3_FILE = 205,
	RTQA_TID_RFID_DG4_FILE = 206,
	RTQA_TID_RFID_DG5_FILE = 207,
	RTQA_TID_RFID_DG6_FILE = 208,
	RTQA_TID_RFID_DG7_FILE = 209,
	RTQA_TID_RFID_DG8_FILE = 210,
	RTQA_TID_RFID_DG9_FILE = 211,
	RTQA_TID_RFID_DG10_FILE = 212,
	RTQA_TID_RFID_DG11_FILE = 213,
	RTQA_TID_RFID_DG12_FILE = 214,
	RTQA_TID_RFID_DG13_FILE = 215,
	RTQA_TID_RFID_DG14_FILE = 216,
	RTQA_TID_RFID_DG15_FILE = 217,
	RTQA_TID_RFID_DG16_FILE = 218,
	RTQA_TID_RFID_VALID_DG1_HASH = 219,
	RTQA_TID_RFID_VALID_DG2_HASH = 220,
	RTQA_TID_RFID_VALID_DG3_HASH = 221,
	RTQA_TID_RFID_VALID_DG4_HASH = 222,
	RTQA_TID_RFID_VALID_DG5_HASH = 223,
	RTQA_TID_RFID_VALID_DG6_HASH = 224,
	RTQA_TID_RFID_VALID_DG7_HASH = 225,
	RTQA_TID_RFID_VALID_DG8_HASH = 226,
	RTQA_TID_RFID_VALID_DG9_HASH = 227,
	RTQA_TID_RFID_VALID_DG10_HASH = 228,
	RTQA_TID_RFID_VALID_DG11_HASH = 229,
	RTQA_TID_RFID_VALID_DG12_HASH = 230,
	RTQA_TID_RFID_VALID_DG13_HASH = 231,
	RTQA_TID_RFID_VALID_DG14_HASH = 232,
	RTQA_TID_RFID_VALID_DG15_HASH = 233,
	RTQA_TID_RFID_VALID_DG16_HASH = 234,
	RTQA_TID_RFID_VALID_SIGNED_ATTRS = 235,
	RTQA_TID_RFID_VALID_SIGNATURE = 236,
	RTQA_TID_RFID_VALID_DG1_DATA = 237,
	RTQA_TID_RFID_VALID_DG2_DATA = 238,
	//RTQA_TID_RFID_VALID_DG3_DATA = 239,
	//RTQA_TID_RFID_VALID_DG4_DATA = 240,
	//RTQA_TID_RFID_VALID_DG5_DATA = 241,
	//RTQA_TID_RFID_VALID_DG6_DATA = 242,
	//RTQA_TID_RFID_VALID_DG7_DATA = 243,
	//RTQA_TID_RFID_VALID_DG8_DATA = 244,
	//RTQA_TID_RFID_VALID_DG9_DATA = 245,
	//RTQA_TID_RFID_VALID_DG10_DATA = 246,
	//RTQA_TID_RFID_VALID_DG11_DATA = 247,
	//RTQA_TID_RFID_VALID_DG12_DATA = 248,
	//RTQA_TID_RFID_VALID_DG13_DATA = 249,
	//RTQA_TID_RFID_VALID_DG14_DATA = 250,
	//RTQA_TID_RFID_VALID_DG15_DATA = 251,
	//RTQA_TID_RFID_VALID_DG16_DATA = 252,

	RTQA_TID_RFID_DG1_EQUALS_MRZ_CHARS = 253,

	RTQA_TID_RFID_ACTIVE_AUTHENTICATION = 254,
	RTQA_TID_RFID_VALIDATE_DOC_SIGNER_CERT = 255,

	RTQA_TID_RFID_CHIP_AUTH_STATUS = 256,
	RTQA_TID_RFID_TERMINAL_AUTH_STATUS = 257,
	RTQA_TID_RFID_EF_CVCA_FILE = 258,

	RTQA_TID_RFID_AIR_BAUD_RATE = 259,
	RTQA_TID_RFID_CHIP_ID = 260

} RTQA_Test;

#define	FIRST_MRZ_TEST		RTQA_TID_MRZ_NUM_OF_CHARS
#define	LAST_MRZ_TEST		RTQA_TID_MRZ_CODELINE_VALID
#define	FIRST_RFID_TEST		RTQA_TID_RFID_BAC
#define	LAST_RFID_TEST		RTQA_TID_RFID_CHIP_ID


enum RTQA_Result
{
	RTQA_R_FAIL = 0,
	RTQA_R_PASS = 1,
	RTQA_R_CONFIMATION_NEEDED = 2,
	RTQA_R_ERROR = 3
};

typedef struct RTQA_MRZCodelineData
{
	int puCodelineNum;
	void *puData;
}RTQA_MRZCodelineData;

typedef struct RTQA_TestResult
{
	RTQA_TestID puTestID;
	BSTR puTestGroupName;
	BSTR puName;
	RTQA_Result puResult;
//	void *aData;
//	RTQA_DataType aDataType;

} RTQA_TestResult;

typedef struct RTQA_CodelineTestResult
{
	RTQA_TestID puTestID;
	RTQA_Result puResult;
	int puCodelineNum;
//	void *aData;
//	RTQA_DataType aDataType;

} RTQA_CodelineTestResult;

typedef struct RTQA_TravelDocType
{
	int puID;
	BSTR puDocName;
} RTQA_TravelDocType;
//Used to return a type of travel document

typedef struct RTQA_DecodedDG1Data
{
	/// Provides the full OCR codeline as a single string.
	char Data[MAX_CODELINE_LENGTH];
	
	/// Provides the number of separate lines found in #Data.
	int LineCount;
	
	/// Provides the first line of the codeline.
	char Line1[MAX_CODELINE_LENGTH];
	
	/// Provides the second line of the codeline.
	char Line2[MAX_CODELINE_LENGTH];
	
	/// Provides the third line of the codeline.
	char Line3[MAX_CODELINE_LENGTH];
	
	/// Provides the identifier of this document, if available.
	///
	/// \remarks If not an empty string, this can include one of the following values:
	///		- \c "SHORTVISA"
	///		- \c "LONGVISA"
	///		- \c "PASSPORT"
	///		- \c "IDTWOLINE"
	///		- \c "IDTHREELINE"
	///		- \c "IDFRANCE"
	///		- \c "USSPECIAL"
	///		- \c "IDHONGKONG"
	///		- \c "RESIDENCECARD"
	///		- \c "SWISSDL"
	///		- \c "UNKNOWN"
	char DocId[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the document type extracted from the codeline.
	///
	/// \remarks This can be one of the following values:
	///		- \c "VISA"
	///		- \c "RESIDENCE CARD"
	///		- \c "PASSPORT"
	///		- \c "IDENTITY CARD"
	///		- \c "DRIVING LICENCE"
	///     - \c "VEHICLE REGISTRATION"
	///		- \c "UNKNOWN DOCUMENT"
	char DocType[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the surname of the document owner read from the codeline, if available.
	char Surname[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the first name of the document owner read from the codeline, if available.
	char Forename[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the second name of the document owner read from the codeline, if available.
	char SecondName[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the first and second names of the document owner read from the codeline, if 
	/// available.
	char Forenames[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the date of birth of the document owner, if available.
	RTQA_ReaderDate DateOfBirth;
	
	/// Provides the expiry date of the document, if available.
	RTQA_ReaderDate ExpiryDate;
	
	/// Provides the three letter ISO 3166-1 country code of the document's issuing state, 
	/// if available.
	char IssuingState[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the three letter ISO 3166-1 country code of the document owner's nationality, 
	/// if available.
	char Nationality[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides the document number, if available.
	char DocNumber[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides a full texual description of the document owner's gender, if available.
	///
	/// \remarks If not an empty string, this can be one of the following values:
	///		- \c "Male"
	///		- \c "Female"
	///		- \c "Unknown"
	char Sex[MAX_CODELINE_FIELD_LENGTH];
	
	/// Provides a single initial from the #Sex field, if available.
	///
	/// \remarks This will be one of the following values:
	///		- \c 'M' for \c "Male"
	///		- \c 'F' for \c "Female"
	///		- \c 'U' for \c "Unknown"
	char ShortSex;
	
	/// Provides the first optional data field, if available.
	char OptionalData1[MAX_OPTIONAL_DATA_LENGTH];

	/// Provides the second optional data field, if available.
	char OptionalData2[MAX_OPTIONAL_DATA_LENGTH];

} RTQA_DecodedDG1Data;

enum RTQA_ImageType
{
	RTQA_IT_IR,
	RTQA_IT_VISIBLE,
	RTQA_IT_UV,
	RTQA_IT_RESERVED1,
	RTQA_IT_RESERVED2
};
#define RTQA_IT_COUNT 5

typedef enum RTQA_ImageFormat
{
	RTQA_IF_GDIPLUSBITMAP,
	RTQA_IF_JPEG,
	RTQA_IF_PNG,
	RTQA_IF_BMP

} RTQA_ImageFormat;

/////////////////////////////////////////////////////////////////////////////
typedef enum RTQAErrorCode
{
	//Use 16-bits of custom error codes left by MMMReaderErrorCode enum
	//CUSTOM_ERROR_CODE_MIN = 0x10000,

	RTQAEC_ERROR_UNKNOWN_ERROR_CODE = 0x10001,

	RTQAEC_ERROR_CALLING_FUNCTION_IN_BLOCKING_MODE,
	RTQAEC_ERROR_CALLING_FUNCTION_IN_NONBLOCKING_MODE,
	
	RTQAEC_ERROR_INITIALISING_MSXML,
	RTQAEC_ERROR_DOC_TYPE_NOT_FOUND,
	RTQAEC_ERROR_LOADING_DOC_TYPE_TOO_MANY_CODELINES,
	RTQAEC_ERROR_LOADING_DOC_TYPE,
	RTQAEC_ERROR_LOADING_DOC_TYPE_UNKNOWN_ELEMENT,
	RTQAEC_ERROR_LOADING_DOC_TYPE_UNKNOWN_FILE_FORMAT,

	RTQAEC_INVALID_VALIDATION_PARAMS,
	RTQAEC_ERROR_GETTING_RFID_DATA,
	RTQAEC_ERROR_UNABLE_TO_QA_RFID_DATA_TYPE,
	RTQAEC_ERROR_DOC_TYPE_VALIDATION_ROUTINE_ERROR,

	RTQAEC_ERROR_QA_RESULT_SET_AT_INCORRECT_TIME,
	RTQAEC_ERROR_QA_DOC_CALLED_BEFORE_DOC_PLACED_ON_WINDOW,

	RTQAEC_ERROR_LIST_DATA_REQUESTED_WITHOUT_INDEX,
	RTQAEC_ERROR_NON_LIST_DATA_REQUESTED_WITH_INDEX,
	RTQAEC_ERROR_UNKNOWN_DATA_TYPE_REQUESTED
	
	//CUSTOM_ERROR_CODE_MAX = 0x1ffff,

} RTQAErrorCode;

#define RTQAEC_ERROR_CODE_COUNT 18
/////////////////////////////////////////////////////////////////////////////

typedef struct RTQAErrorMessage
{
	int puErrorCode;
	char *puErrorMessage;
} RTQAErrorMessage;


typedef void (*RTQA_DataCallback)(
	void *aParam, 
	RTQA_DataType aDataType, 
	int aDataLen, 
	void *aDataPtr
);
//Callback used for passing data between QA API and an Application

typedef void (*RTQA_EventCallback)(
	void *aParam,
	RTQA_EventType aEventType
);
//Callback used by QA API for notifying an Application an event has occured

typedef void (*RTQA_ProgressCallback)(
	void *aCallbackParam,
	float aProgressPercentage
);

#endif //_RTQADataTypes_H_