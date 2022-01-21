#ifndef _RTQAHIGHLEVELINTERFACE_H
#define _RTQAHIGHLEVELINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include "RTQADataTypes.h"
#include "MMMReaderDataTypes.h"

#ifdef RTQAHIGHLEVELAPI_EXPORTS
	#define RT_QA_HIGHLEVEL_API __declspec(dllexport)
#else
	#define RT_QA_HIGHLEVEL_API __declspec(dllimport)
#endif //RTQAHIGHLEVELAPI_EXPORTS

/////////////////////////////////////////////////////////////////////////////
//	High-Level QA funtions
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
//	Common Blocking/Non-Blocking Functions
//---------------------------------------------------------------------------

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_Terminate(
);
//Call this function to close the reader and clean up all internal memory
//used by the API
//
//Returns - NO_ERROR_OCCURRED if API was successfully terminated


RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_EnableAuditLogging(
	BOOL aEnabled, 
	char *aFilename
);
// Turn on/off the audit logging of pass/fail results for given documents.
// aEnabled - Whether the audit log is turned on or off
// aFilename - File path where the data will be saved.

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_EnableLogging(
	BOOL aEnabled,
	int aLogLevel,
	int aLogMask,
	char *aFilename
);
//Turn logging on/off and set the type of things to log
//
//aEnabled - Whether logging is turned on/off 
//aLogLevel - Level of logging: 0 = No data, 5 = Full logging data
//aLogMask - Mask to indicate which types of data to log
//aFilename - File path where the data will be saved
//
//Returns - NO_ERROR_OCCURRED if logging was successfully set

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_SetLoggingOptions(
);
//Setup the logging strategy per settings - needs to be called after Initialization
//
//Returns - NO_ERROR_OCCURRED if successfully set

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_GetErrorMessage(
	MMMReaderErrorCode aErrorCode, 
	char *aErrorString,
	int *aStrLen
);
//Get the text associated with a given error code
//
//aErrorCode - The error code who's text to return
//aErrorString - Buffer to receive the error message. If the buffer is
//               not big enough not data will be placed into it.
//aStrLen - The pointer to length of the buffer aErrorString. The value
//          pointed to will be changed to be the exact length of the the
//          error string found.
//
//Returns - NO_ERROR_OCCURRED if error message was successfully returned
//          ERROR_STRING_BUFFER_TOO_SMALL if aStrLen is less than the size of
//          the string found

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_ImageConvertFormat(
	int aDestinationFormat, 
	unsigned char *aInBuffer, 
	int aInBufferLen, 
	char *aOutBuffer, 
	int *aOutBufferLen
);
//Converts an image from one format into another.
//
//aDestinationFormat - Specify which format the source image should be converted to.
//                     Should be a MMMReaderImageFormats value.
//aInBuffer - A pointer to the raw image data to convert from.
//aInBufferLen - The size of aInBuffer in bytes.
//aOutBuffer - A pointer to a writable buffer which will be updated to store the
//             converted image. This can be NULL if you wish to find out the size
//aOutBufferLen - Set this to the length of aOutBuffer. If aOutBuffer is large
//                enough to hold the converted image, it will be filled.
//                When the function returns, this value will be the actual size 
//                of the resulting image
//
//Returns - NO_ERROR_OCCURRED if validation was carried out without error

//---------------------------------------------------------------------------
//	BLOCKING MODE FUNCTIONS
//---------------------------------------------------------------------------

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_BM_Initialise(
	const char *aDocTypesFilePath,
	BOOL aProcessMessages,
	BOOL aFilterInputMessages
);
//Use this function to initialise the High-Level QA API in Blocking Mode.
//When in blocking mode all functions will return only after all their tasks
//have been carried out. The data that was read is obtained by calling the
//GetData functions.
//
//aDocTypesFilePath - A Null termiated string specifying the file path of the 
//                    document type XML file to use for validation.
//aProcessMessages - If TRUE Windows messages will be passed on while
//                   blocking
//aFilterInputMessages - If true input messages (Mouse/Keyboard messages)
//                       won't be passed on, when aProcessMessages is true.
//                       If aProcessMessage if false, this setting has no 
//                       affect.
//
//Returns - NO_ERROR_OCCURRED if API was successfully initialised

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_BM_RequestTravelDocList(
);
//Creates a list of all the travel documents currently loaded from the 
//DocTypes XML file supplied to the initialise function. Use the GetData
//functions get hold of the data.
//
//Returns - NO_ERROR_OCCURRED if a list of travel document types was
//          successfully created

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_BM_WaitForDocumentOnScanner(
	int aTimeout
);
//Waits until a document has been detected on the scanner window
//
//aTimeout - If no document has been place on the scanner window within this
//           time, the function will return anyway. (Milliseconds)
//
//Returns - NO_ERROR_OCCURRED if document was successfully detected
//          ERROR_TIMED_OUT if aTimeout was reached

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_BM_QADocumentOnScanner(
	int aDocumentType
);
//Validates the document currently on scanner, according to tests specified
//in document type XML file. Use the GetData functions to obtain the data
//read from the document and the test results from the document QA.
//
//aDocumentType - Specifies the document expected and therefore the tests that
//                will be carried out.
//
//Returns - NO_ERROR_OCCURRED if document QA was successfully carried out

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_BM_GetData(
	RTQA_DataType aDataType, 
	void **aDataPtr, 
	int &aDataLen
);
//Returns data that has been stored as a result of calling one of the other
//API functions. When RelaseResources is called, or a new document QA process
//is started, all the data will be emptied.
//
//aDataType - The data to get 
//aDataPtr - Gets set to the data requested
//aDataLen - Gets set to the length of the data requested
//
//Returns - NO_ERROR_OCCURRED If the data was successfully returned

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_BM_GetListDataByIndex(
	RTQA_DataType aDataType, 
	int aIndex,
	void **aDataPtr, 
	int &aDataLen
);
//Returns data that has been stored as a result of calling one of the other
//API functions. Use this function when the data needed is from a list
//e.g. QA_DT_TRAVEL_DOC_TYPE
//All list data items will have a corrisponding count data item, so you know
//how many elements there are. e.g. QA_DT_TRAVEL_DOC_TYPE_COUNT
//
//aDataType - The data to get 
//aIndex - The index of the of the item in the list
//aDataPtr - Gets set to the data requested
//aDataLen - Gets set to the length of the data requested
//
//Returns - NO_ERROR_OCCURRED If the data was successfully returned


//---------------------------------------------------------------------------
//	NON-BLOCKING MODE FUNCTIONS
//---------------------------------------------------------------------------

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_NBM_Initialise(
	const char *aDocTypesFilePath,
	RTQA_DataCallback aDataCallback,
	RTQA_EventCallback aEventCallback,
	MMMReaderErrorCallback aErrorCallback,
	RTQA_ProgressCallback aProgressCallback,
	void *aCallbackParam
);
//Use this function to initialise the High-Level QA API in Non-Blocking Mode.
//When in non-blocking mode all data is passed back to the application via
//callbacks. These callbacks may be triggered after the function that
//requested them has returned. 
// e.g. RTQAHL_NBM_RequestTravelDocList() may return before any
//      RTQA_DataCallback functions have been called.
//
//aDocTypesFilePath - A Null termiated string specifying the file path of the 
//                    document type XML file to use for validation.
//aDataCallback - A callback function to receive the data requested through
//                other API calls.
//aEventCallback - A callback function to receive event notifications
//aErrorCallback - A callback function to receive errors
//aProgressCallback - A callback function to receive progress information
//                    when carrying out document QA
//aCallbackParam - Parameter passed to Data, Event and Progress callback
//                 functions
//
//Returns - NO_ERROR_OCCURRED if API was successfully initialised

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_NBM_QADocumentsPlacedOnScanner(
	int aDocType
);
//Request QA of all documents placed on scanner, as a particular document
//type. All data read/results of test will be sent to the DataCallback
//function. The EventCallback function will be told when the different stages
//of QA are completed. 
//Once the QA_ET_DOC_DATA_RESULTS_END message has been received, the
//application must call the RTQAHL_NBM_SetDocumentQAResult() function to set
//the result. Only once this has been called will another document start the
//QA process. 
//NB. Once a new QA process is started, all data from the previous one is
//    deleted.
//
//aDocType - The type of document who's QA tests to carry out
//
//Returns - NO_ERROR_OCCURRED if the QA process was successfully launched

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_NBM_IgnoreDocumentsPlacedOnScanner(
);
//Request that nothing happens when documents are placed/removed from the
//scanner window.
//
//Returns - NO_ERROR_OCCURRED if the Ignore state has been successfully
//          requested

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_NBM_SetDocumentQAResult(
	BOOL aPass
);
//Set the result of the document currently going through the QA process. This
//will trigger a new QA process to begin, which will free all memory used for
//the previous QA process.
//
//aPass - The result of the QA process
//
//Returns NO_ERROR_OCCURRED if the result was set successfully
//        RTQAEC_ERROR_QA_RESULT_SET_AT_INCORRECT_TIME if the function was
//        called at an incorrect time

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_NBM_RequestTravelDocList(
);
//Use this function to get a list of the document types that were loaded from
//the XML file during the Initialise step. Each document ID and name will be
//sent to the DataCallback funtion in the form of a RTQA_TravelDocType. When
//all the document types have been returned the EventCallback funtion is sent
//a QA_ET_DOC_TYPES_LIST_END event.
//
//Returns - NO_ERROR_OCCURRED if the travel document list has been
//          successfully requested

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_NBM_ChangeCallbacks(
	RTQA_DataCallback aDataCallback,
	RTQA_EventCallback aEventCallback,
	MMMReaderErrorCallback aErrorCallback,
	RTQA_ProgressCallback aProgressCallback,
	void *aCallbackParam
);
//Use this function to change the callback functions called
//
//aDataCallback - A callback function to receive the data requested through
//                other API calls.
//aEventCallback - A callback function to receive event notifications
//aErrorCallback - A callback function to receive errors
//aProgressCallback - A callback function to receive progress information
//                    when carrying out document QA
//aCallbackParam - Parameter passed to Data, Event and Progress callback
//                 functions
//
//Returns - NO_ERROR_OCCURRED if the callback functions were successfully
//          changed

RT_QA_HIGHLEVEL_API MMMReaderErrorCode RTQAHL_NBM_ForceRead();

#ifdef __cplusplus
 } 
#endif //__cplusplus

#endif //_RTQAHIGHLEVELINTERFACE_H