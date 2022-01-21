#ifndef _MMMREADERHIGHLEVELAPI_H_
#define _MMMREADERHIGHLEVELAPI_H_

//#define MMMREADERHIGHLEVELAPI_EXPORTS

#ifdef __cplusplus
extern "C" {
#endif

#include "MMMReaderDataTypes.h"
#include "MMMReaderSettings.h"

#ifdef MMMREADERHIGHLEVELAPI_EXPORTS
#if defined(__linux__) || defined(__APPLE__)
#define MMM_READER_HIGH_LEVEL_API __attribute__ ((visibility("default")))
#else
#define MMM_READER_HIGH_LEVEL_API __declspec(dllexport)
#endif
#elif defined(MMMREADERHIGHLEVELAPI_STATIC_LINK)
#define MMM_READER_HIGH_LEVEL_API
#else
#if defined(__linux__) || defined(__APPLE__)
#define MMM_READER_HIGH_LEVEL_API 
#else
#define MMM_READER_HIGH_LEVEL_API __declspec(dllimport)
#endif
#endif

/// Defines all the data items that the \hlapi can process and return to the host application.
///
/// \ingroup HighLevelDataModule
///
/// \note These constants are only relevant if using the \hlapi.
///
/// \remarks For the Blocking API, these values are passed into MMMReader_GetData() to request
/// specific data back.
///
/// \remarks For the Non-Blocking API, these values are returned via the 
/// #MMMReaderHLDataCallback passed into the MMMReader_Initialise() API.
///
/// \remarks Regardless of which API set is used, all data is returned as a \c void* (hereby
/// referred to as \a aDataPtr), with the size of the data also returned (hereby referred to as
/// \a aDataLen). The \a aDataPtr pointer must be cast to an appropriate type before
/// the data can be used; details of which type to cast to are described below.
///
/// \remarks As an example, to get and cast data using the Blocking API:
///
/// \remarks \include HLGetDataBlocking.cpp
///
/// \remarks To get and cast data using the Non-Blocking API:
///
/// \remarks \include HLGetDataNonBlocking.cpp
typedef enum MMMReaderDataType
{
	/// The OCR ICAO codeline read from the Machine Readable Zone (MRZ) of a document.
	///
	/// \note Although still supported, it is recommended to use the #CD_CODELINE_DATA data item
	/// instead. This provides the same functionality as #CD_CODELINE as well as other features
	/// for ease-of-use.
	///
	/// \remarks The data returned will be a pointer to a null-terminated ASCII string, with
	/// a maximum length of 200 characters, containing up to three lines of OCR data. The lines
	/// are separated with carriage returns (ASCII \c 0x0D).
	///
	/// \remarks Cast \a aDataPtr to a \c char* to use the data. \a aDataLen will represent
	/// the length of the string.
	///
	/// \todo Document enabling data item
	CD_CODELINE = 0,
	
	/// A MMMReaderCodelineData structure containing all parsed data from the OCR ICAO codeline
	/// read from the Machine Readable Zone (MRZ) of a document.
	///
	/// \note This data item differs from #CD_CODELINE in that more data is returned in the
	/// MMMReaderCodelineData structure. The \sdk will parse the OCR data to split up the lines
	/// and extract common data fields, such as names and dates. Therefore, using this data
	/// item is recommended over using #CD_CODELINE.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderCodelineData*. \a aDataLen will represent 
	/// the size of the structure in bytes.
	///
	/// \todo Document enabling data item
	CD_CODELINE_DATA,
	
	/// A result value indicating the result of computing a checksum over the OCR codeline.
	///
	/// \remarks Cast \a aDataPtr to an \c int*; the value is represented as a single 32-bit
	/// integer. \a aDataLen will represent the size of an integer.
	///
	/// \remarks #CD_CHECKSUM can be one of the following values:
	/// <table>
	/// <tr><th>Value</th><th>Description</th></tr>
	///
	/// <tr><td>0</td><td>Checksum not checked</td></tr>
	///
	/// <tr><td>&gt; 0</td><td>Checksum OK</td></tr>
	///
	/// <tr><td>-1</td><td>Checksum error</td></tr>
	///
	/// <tr><td>&lt; -1</td><td>Checksum warning</td></tr>
	///
	/// </table>
	///
	/// \remarks The checksum warning signifies that the checksum did not compute but that
	/// the document is not an ICAO document, so it could use different checksum rules. Warnings
	/// can be produced from some three-line US documents.
	///
	/// \todo Document enabling data item
	CD_CHECKSUM,
	
	/// \todo Find out what this does
	CD_CHECKSUMEXTENDED,
	
	/// The infra-red (IR) image snapshot taken of the document.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The IR image is cropped to the document edges, rotated if necessary and 
	/// converted to grayscale.
	///
	/// \todo Document enabling data item and "special" mode
	CD_IMAGEIR,

	/// The infra-red (IR) image snapshot taken of the document for the split side..
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The IR image is cropped to the document edges, rotated if necessary and 
	/// converted to grayscale.
	///
	CD_IMAGEIRREAR,
	
	/// The visible image snapshot taken of the document.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The visible image is cropped to the document edges and rotated if necessary.
	///
	/// \todo Document enabling data item and "special" mode
	CD_IMAGEVIS,

	/// The visible image snapshot taken of the rear of the document or split side.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The visible image is cropped to the document edges and rotated if necessary, CD_IMAGEVISREAR is for backwards compatiablity to ID150.
	///
	CD_IMAGEVISREAR,

	/// Similar to #CD_IMAGEVIS, except this visible image snapshot is taken with different
	/// lighting parameters to either highlight or suppress Optically Variable Device
	/// features within the document laminate. This image is currently only valid on some RTE8000
	/// scanners and all RTE8000 HS units, and is not a fully calibrated "high quality" image.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The visible image is cropped to the document edges and rotated if necessary.
	///
	/// \todo Document enabling data item
	CD_IMAGEVIS_OVD1,
	
	/// Similar to #CD_IMAGEVIS, except this visible image snapshot is taken with different
	/// lighting parameters to either highlight or suppress Optically Variable Device
	/// features within the document laminate. This image is currently only valid on some RTE8000
	/// scanners and all RTE8000 HS units, and is not a fully calibrated "high quality" image.
	///
	/// \note This image uses different lighting parameters to #CD_IMAGEVIS_OVD1.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The visible image is cropped to the document edges and rotated if necessary.
	///
	/// \todo Document enabling data item
	CD_IMAGEVIS_OVD2,
	
	/// The photo image that is cropped from #CD_IMAGEVIS. Face find is used to locate and crop the photo.
	/// If not face is found, then this is the fixed part of the 
	/// document where a photo of the document owner can usually be found.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The photo image is cropped to a face, if found, or relative to the document's bottom left edge and
	/// rotated if necessary.
	///
	/// \todo Document enabling data item and "special" mode
	CD_IMAGEPHOTO,
	
	/// The ultraviolet (UV) image snapshot taken of the document.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The UV image is cropped to the document edges and rotated if necessary.
	///
	/// \todo Document enabling data item and "special" mode
	CD_IMAGEUV,	
	
	/// The ultraviolet (UV) image snapshot taken of the document for the split side.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The UV image is cropped to the document edges and rotated if necessary.
	///
	/// \todo Document enabling data item and "special" mode
	CD_IMAGEUVREAR,
	
	/// The coaxial visible image snapshot taken of the document.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The COAX Vis image is cropped to the document edges and rotated if necessary.
	/// This image is useful for revealing the presence of a Confirm security laminate.
	///
	/// \todo Document enabling data item 
	CD_IMAGECOAXVIS,	

	/// The coaxial IR image snapshot taken of the document.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The COAX IR image is cropped to the document edges and rotated if necessary.
	/// This image is useful for highlighting damage to a Confirm security laminate.
	///
	/// \todo Document enabling data item 
	CD_IMAGECOAXIR,	
		
	/// An optimised visible image that is used exclusively by the \sdk barcode plugin
	/// decoders.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks From the perspective of the user, this image provides no real use. However,
	/// it is useful for diagnosing problems with reading barcodes as this is the exact image
	/// used by the \sdk decoders. Therefore, tests can be replicated on this image.
	///
	/// \remarks This image is only returned when at least one barcode plugin is enabled,
	/// regardless of whether the data item has been enabled.
	///
	/// \todo Document enabling data item
	CD_IMAGEBARCODE,
	
	/// An optimised visible rear image that is used exclusively by the \sdk barcode plugin
	/// decoders.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks From the perspective of the user, this image provides no real use. However,
	/// it is useful for diagnosing problems with reading barcodes as this is the exact image
	/// used by the \sdk decoders. Therefore, tests can be replicated on this image.
	///
	/// \remarks This image is only returned when at least one barcode plugin is enabled,
	/// regardless of whether the data item has been enabled.
	///
	/// \todo Document enabling data item
	CD_IMAGEBARCODEREAR,
	
	/// A result which indicates whether the document has passed IR and UV security checks.
	///
	/// \remarks Cast \a aDataPtr to an \c int*; the value is represented as a single 32-bit
	/// integer. \a aDataLen will represent the size of an integer.
	///
	/// \remarks Bit 0 represents the IR check, while bit 1 represents the UV check. If the bit
	/// for that check is set, then that particular test failed. For example:
	///
	/// \remarks \code 
	/// int lSecurityCheck = // Get this value from CD_SECURITYCHECK...
	/// if (lSecurityCheck & 3)
	///		printf("Failed");
	/// else
	///		printf("Success");
	/// \endcode
	///
	/// \remarks If the document fails the IR check, the UV check will not be carried out as
	/// this takes time.
	///
	/// \todo Document enabling data item and "special" mode
	CD_SECURITYCHECK,

	/// The decoded data retrieved from the DG1 data group of an RFID chip. This represents
	/// the e-Passport's Machine Readable Zone (MRZ) data, and is comparable to #CD_CODELINE.
	///
	/// \note Although still supported, it is recommended to use the 
	/// #CD_SCDG1_CODELINE_DATA data item instead. This provides the same functionality as 
	/// #CD_SCDG1_CODELINE as well as other features for ease-of-use.
	///
	/// \remarks The data returned will be a pointer to a null-terminated ASCII string, with
	/// a maximum length of 200 characters, containing up to three lines of OCR data. Unlike
	/// #CD_CODELINE, the lines are <em>not</em> separated with carriage returns 
	/// (ASCII \c 0x0D) to indicate line breaks.
	///
	/// \remarks Cast \a aDataPtr to a \c char* to use the data. \a aDataLen will represent
	/// the length of the string.
	///
	/// \todo Document enabling data item
	CD_SCDG1_CODELINE,
	
	/// A MMMReaderCodelineData structure containing all parsed data from the decoded data
	/// retrieved from the DG1 data group of an RFID chip. This is comparable to 
	/// #CD_CODELINE_DATA.
	///
	/// \note This data item differs from #CD_SCDG1_CODELINE in that more data is returned in 
	/// the MMMReaderCodelineData structure. The \sdk will parse the MRZ data to split up the 
	/// lines and extract common data fields, such as names and dates. Therefore, using this 
	/// data item is recommended over using #CD_SCDG1_CODELINE.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderCodelineData*. \a aDataLen will represent 
	/// the size of the structure in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG1_CODELINE_DATA,	// RF - codeline data fields, extracted from CD_SCDG1_CODELINE
	
	/// The decoded data retrieved from the DG2 data group of an RFID chip. This represents
	/// the e-Passport photo.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The photo image will be in either the JPEG or JPEG 2000 format, depending
	/// on the e-Passport's country of origin.
	///
	/// \remarks The data is formatted such that it can be saved straight to disk if required.
	///
	/// \todo Document enabling data item
	CD_SCDG2_PHOTO,
	
	/// \eac The decoded data retrieved from the DG3 data group of an RFID chip. This 
	/// represents the e-Passport's biometric fingerprint data.
	///
	/// \note This data item is only returned if Terminal Authentication is successful.
	///
	/// \remarks Cast \a aDataPtr to a \c DG3FingerprintData*. \a aDataLen will represent the 
	/// size of the structure in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG3_FINGERPRINTS,
	
	/// The result of validating the DG1 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG1_VALIDATE,
	
	/// The result of validating the DG2 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG2_VALIDATE,
	
	/// The result of validating the DG3 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG3_VALIDATE,
	
	/// The result of validating the DG4 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG4_VALIDATE,
	
	/// The result of validating the DG5 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG5_VALIDATE,
	
	/// The result of validating the DG6 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG6_VALIDATE,
	
	/// The result of validating the DG7 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG7_VALIDATE,
	
	/// The result of validating the DG8 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG8_VALIDATE,
	
	/// The result of validating the DG9 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG9_VALIDATE,
	
	/// The result of validating the DG10 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG10_VALIDATE,
	
	/// The result of validating the DG11 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG11_VALIDATE,
	
	/// The result of validating the DG12 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG12_VALIDATE,
	
	/// The result of validating the DG13 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG13_VALIDATE,
	
	/// The result of validating the DG14 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG14_VALIDATE,
	
	/// The result of validating the DG15 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG15_VALIDATE,
	
	/// The result of validating the DG16 file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG16_VALIDATE,
	
	/// The result of validating the hash of the e-Passport contents against the Signed
	/// Attributes from an RFID chip using the EF.SoD.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCSIGNEDATTRS_VALIDATE,
	
	/// The result of validating the signature in the EF.SoD Document Security Object using a
	/// public key from the Document Signer Certificate (DSC) for an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item and cross-reference with DocSignerCertMode
	/// settings
	CD_SCSIGNATURE_VALIDATE,
	
	/// The over-air baud rate used for RFID chip communications.
	///
	/// \remarks Cast \a aDataPtr to a \c char*; this represents a null-terminated string.
	/// \a aDataLen will represent the length of the string.
	///
	/// \remarks This string will typically contain one of the following values:
	///		- \c "848"
	///		- \c "424"
	///		- \c "212"
	///		- \c "106"
	///
	/// \todo Document enabling data item
	CD_SCAIRBAUD,
	
	/// The chip ID used during RFID chip communications.
	///
	/// \remarks Cast \a aDataPtr to a \c char*; this represents a null-terminated string.
	/// \a aDataLen will represent the length of the string.
	///
	/// \todo Document enabling data item
	CD_SCCHIPID,
	
	/// The EF.COM file retrieved from an RFID chip, which contains the Data Group Presence
	/// Map.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \remarks It is likely that this will be the first file read from an RFID chip, even
	/// if the data item is not enabled.
	///
	/// \remarks For e-Passports with Basic Access Control (BAC), the reading of this file
	/// will trigger the establishment of BAC, hence it will take longer to read the file
	/// than expected.
	///
	/// \todo Document enabling data item
	CD_SCEF_COM_FILE,
	
	/// The EF.SOD file retrieved from an RFID chip, which contains the Document Security
	/// Object and optionally the Document Signer Certificate (DSC).
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCEF_SOD_FILE,
	
	/// The raw DG1 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG1_FILE,
	
	/// The raw DG2 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG2_FILE,
	
	/// The raw DG3 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG3_FILE,
	
	/// The raw DG4 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG4_FILE,
	
	/// The raw DG5 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG5_FILE,
	
	/// The raw DG6 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG6_FILE,
	
	/// The raw DG7 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG7_FILE,
	
	/// The raw DG8 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG8_FILE,
	
	/// The raw DG9 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG9_FILE,
	
	/// The raw DG10 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG10_FILE,
	
	/// The raw DG11 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG11_FILE,
	
	/// The raw DG12 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG12_FILE,
	
	/// The raw DG13 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG13_FILE,
	
	/// The raw DG14 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG14_FILE,
	
	/// The raw DG15 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG15_FILE,
	
	/// The raw DG16 file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG16_FILE,	
	
	/// \eac The EF.CVCA file retrieved from an RFID chip, which contains a list of
	/// Certificate Authority References (CARs) to identify the public key to be used in
	/// the Terminal Authentication stage of Extended Access Control (EAC).
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \remarks This file should only be present in e-Passports with EAC.
	///
	/// \todo Document enabling data item
	CD_SCEF_CVCA_FILE,
	
	/// A #TRISTATE value indicating whether Basic Access Control (BAC) was used to open
	/// an e-Passport for reading.
	///
	/// \remarks Cast \a aDataPtr to a \c TRISTATE*; this represents a single result. 
	/// \a aDataLen will represent the size of #TRISTATE in bytes. See #TRISTATE for the 
	/// possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCBAC_STATUS,
	
	/// A character buffer containing the MRZ used for establishing Basic Access Control (BAC)
	/// for an e-Passport.
	///
	/// \remarks Cast \a aDataPtr to a \c char*; this represents a writable character buffer.
	/// \a aDataLen will represent the maximum size of the buffer.
	///
	/// \remarks This data item works differently to the others in that it requires a response
	/// from the host application. This data item is returned if establishing BAC fails, 
	/// meaning the host application has the opportunity to correct the MRZ by changing the
	/// recieved buffer.
	///
	/// \remarks Below is an example of how to do this:
	///
	/// \remarks \include BACKeyCorrection.cpp
	///
	/// \attention The #MMMReaderHLDataCallback will be called from a different thread to
	/// that used normally, and the callback may be fired independently on another thread
	/// compared to non-RFID items. No further RFID data processing will continue until the
	/// callback returns.
	///
	/// \todo Document enabling data item and is this only for Non-Blocking?
	CD_BACKEY_CORRECTION,

	/// A #TRISTATE value indicating whether Active Authentication (AA) was performed on
	/// an e-Passport.
	///
	/// \remarks Cast \a aDataPtr to a \c TRISTATE*; this represents a single result. 
	/// \a aDataLen will represent the size of #TRISTATE in bytes. See #TRISTATE for the 
	/// possible values this can represent.
	///
	/// \remarks Active Authentication is an optional security feature of e-Passports which
	/// prevents chip cloning attacks. It will only be performed if DG15 is present.
	///
	/// \todo Document enabling data item
	CD_ACTIVE_AUTHENTICATION,
	
	/// The result of validating the EF.SoD Document Signer Certificate's (DSC) signature using the
	/// public key from an external Country Signer Certificate (CSC) for an e-Passport.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item and cross-reference with ExternalCSCMode
	/// settings
	CD_VALIDATE_DOC_SIGNER_CERT,
	
	/// \eac A #TRISTATE value indicating whether the Terminal Authentication (TA) stage of
	/// Extended Access Control (EAC) was performed on an e-Passport.
	///
	/// \remarks Cast \a aDataPtr to a \c TRISTATE*; this represents a single result. 
	/// \a aDataLen will represent the size of #TRISTATE in bytes. See #TRISTATE for the 
	/// possible values this can represent.
	///
	/// \remarks Terminal Authentication will only be attempted if DG3 or DG4 is accessed
	/// from an EAC enabled RFID chip.
	///
	/// \todo Document enabling data item and cross-reference ExternalCVCertsMode
	CD_SCTERMINAL_AUTHENTICATION_STATUS,
	
	/// \eac A #TRISTATE value indicating whether the Chip Authentication (CA) stage of
	/// Extended Access Control (EAC) was performed on an e-Passport.
	///
	/// \remarks Cast \a aDataPtr to a \c TRISTATE*; this represents a single result. 
	/// \a aDataLen will represent the size of #TRISTATE in bytes. See #TRISTATE for the 
	/// possible values this can represent.
	///
	/// \remarks Chip Authentication will be performed automatically by the \sdk immediately
	/// after reading the EF.COM file if DG14 is present.
	///
	/// \todo Document enabling data item
	CD_SCCHIP_AUTHENTICATION_STATUS, 
	
	CD_SCCROSSCHECK_EFCOM_EFSOD,

	/// A #MMMReaderValidationCode value indicating whether Passive Authentication (PA) was
	/// performed on an e-Passport.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \remarks Passive Authentication is a security feature of e-Passports which proves
	/// a chip is genuine.
	///
	/// \todo Document enabling data item
	CD_PASSIVE_AUTHENTICATION,

	/// A #TRISTATE value indicating whether Supplemental Access Control (BAC) was used to open
	/// an e-Passport for reading.
	///
	/// \remarks Cast \a aDataPtr to a \c TRISTATE*; this represents a single result. 
	/// \a aDataLen will represent the size of #TRISTATE in bytes. See #TRISTATE for the 
	/// possible values this can represent.
	CD_SAC_STATUS,

	/// \eac The EF.CardAccess file retrieved from an SAC enabled RFID chip, which contains 
	/// a list of SecurityInfos  to identify the algorithms to be used for SAC.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \remarks This file should only be present in e-Passports with SAC.
	CD_SCEF_CARD_ACCESS_FILE,

	/// 2 byte bit map of data groups (DG1-DG16) present in the EF.COM.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the 2 bytes
	/// with a bit (1-16) for each DG present in the EF.COM (DG1-DG16).
	/// \a aDataLen will represent the size of the data in bytes.
	///
	CD_EFCOM_DG_MAP,

	/// 2 byte bit map of data group hashes (DG1-DG16) present in the EF.SOD.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the 2 bytes
	/// with a bit (1-16) for each DG hash present in the EF.SOD (DG1-DG16).
	/// \a aDataLen will represent the size of the data in bytes.
	///
	CD_EFSOD_HASH_MAP,

	/// The Document Signer Certificate (DSC) from the EF.SOD, if present.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw cert in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	CD_DOC_SIGNER_CERT,

	/// A MMMReaderMsrData structure containing all parsed data received from a Magnetic
	/// Swipe Reader (MSR) device.
	///
	/// \note This data item is only relevant if using a \swipereader.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderMsrData*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \todo Document enabling data item
	CD_SWIPE_MSR_DATA,
	
	/// A MMMReaderAAMVAData structure containing all parsed data received from the PDF417
	/// barcode of a North American drivers licence which adhears to the \aamva standard.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderAAMVAData*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \todo Document enabling data item and mention enabling PDF417 plugin
	CD_AAMVA_DATA,

	/// \todo Find out if this is still used
	CD_QAINFO,

	/// A #MMMReaderUHFTagIDData structure containing the TagID data read from the
	/// UHF tag in the field of the reader.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderUHFTagIDData*. \a aDataLen will 
	/// represent the size of the structure in bytes.
	CD_UHF_TAGID,

	/// An unsigned char buffer containing the EPC data read from the the UHF tag
	/// in the field of the reader.
	///
	/// \remarks Cast \a aDataPtr to an \c unsigned char*. \a aDataLen will
	/// represent the length of the buffer.
	CD_UHF_EPC,

	/// A #MMMReaderUHFTagMemoryData structure containing some data read from the
	/// UHF tag in the field of the reader.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderUHFTagMemoryData*. \a aDataLen will 
	/// represent the size of the structure in bytes.
	CD_UHF_MEMORY,

	/// A MMMReaderAAMVAData structure containing all parsed data received from a
	/// mag stripe of a North American drivers licence which adhears to the \aamva standard.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderAAMVAData*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks To enable the parsing of mag stripe data make sure #SwipeDataToSend::puAAMVA
	/// is set to 1 in #MMMSwipeSettings::puDataToSend.
	CD_SWIPE_AAMVA_DATA,

	/// Represents the progress of the current document read, as a percentage of completeness.
	///
	/// \remarks Cast \a aDataPtr to a \c float*; this represents a single floating-point
	/// value. \a aDataLen will represent the size of a floating-point value in bytes.
	///
	/// \remarks This value will always be between 0 and 100.
	///
	/// \todo Document enabling data item
	CD_READ_PROGRESS,

	/// \internal
	CD_INSPECTION,

//#ifdef//DCA
	/// A MMMReaderIdentifyResult structure containing the data and results returned after the 
	///  identification stage has completed
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderIdentifyResult*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	CD_IDENTIFIED_RESULT,

	/// A MMMReaderCompletionResult structure containing the data and results returned at the end 
	///  of the document processing
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderCompletionResult*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	CD_COMPLETION_RESULT,

	/// A MMMReaderProcessResultGeneral structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultGeneral*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_UHF, PT_Plugables, PT_Custom
	CD_PROCESS_RESULT_GENERAL,

	/// A MMMReaderProcessResultIcaoMrz structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultIcaoMrz*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_IcaoMrz
	CD_PROCESS_RESULT_ICAOMRZ,

	/// A MMMReaderProcessResultIcaoMrzQA structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultIcaoMrzQA*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_IcaoMrzQA
	CD_PROCESS_RESULT_ICAOMRZQA,

	/// A MMMReaderProcessResultTextCompare structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultTextCompare*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_TextCompare
	CD_PROCESS_RESULT_TEXT_COMPARE,

	/// A MMMReaderProcessResultTextZone structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultTextZone*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_TextZone
	CD_PROCESS_RESULT_TEXT_ZONE,

	/// A MMMReaderProcessResultImageCompare structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultImageCompare*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_PatternMatch, PT_SubImage, PT_Confirm
	CD_PROCESS_RESULT_IMAGE_COMPARE,

	/// A MMMReaderProcessResultImageZone structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultImageZone*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_BlankArea, PT_Tamper, PT_Anchor
	CD_PROCESS_RESULT_IMAGE_ZONE,

	/// A MMMReaderProcessResultBarcode structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultBarcode*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_Barcode
	CD_PROCESS_RESULT_BARCODE,

	/// A MMMReaderProcessResultMagData structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultMagData*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_MagSwipe
	CD_PROCESS_RESULT_MAG,

	/// A MMMReaderProcessResultRF structure containing the data and results for a process.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderProcessResultRF*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will only be returned for process types of PT_RF
	CD_PROCESS_RESULT_RF,

	/// A MMMReaderVerifierResult structure containing the data and results for a verifier.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderVerifierResult*. \a aDataLen will represent the
	/// size of the structure in bytes.
	///
	/// \remarks This structure will be returned for each verifier for the document identified
	CD_VERIFIER_RESULT,
//#endif //DCA

	/// A MMMReaderPhotoData structure containing data relevant to the cropped photo in CD_IMAGEPHOTO.
	/// This data can be useful if custom cropping of the face/photo is desired.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderPhotoData*. \a aDataLen will represent 
	/// the size of the structure in bytes.
	///
	CD_IMAGEPHOTODATA,

	CD_SCDG1_EID_DOCUMENT_TYPE,
	CD_SCDG2_EID_ISSUING_ENTITY,
	CD_SCDG3_EID_VALIDITY_PERIOD,
	CD_SCDG4_EID_GIVEN_NAMES,
	CD_SCDG5_EID_FAMILY_NAMES,
	CD_SCDG6_EID_NOM_DE_PLUME,
	CD_SCDG7_EID_ACADEMIC_TITLE,
	CD_SCDG8_EID_DATE_OF_BIRTH,
	CD_SCDG9_EID_PLACE_OF_BIRTH,
	CD_SCDG10_EID_NATIONALITY,
	CD_SCDG11_EID_SEX,
	CD_SCDG12_EID_OPTIONAL_DATA_R,
	CD_SCDG13_EID_BIRTH_NAME,
	CD_SCDG14_EID_WRITTEN_SIGNATURE,
	CD_SCDG17_EID_PLACE_OF_RESIDENCE,
	CD_SCDG18_EID_MUNICIPALITY_ID,
	CD_SCDG19_EID_RESIDENCE_PERMIT_1,
	CD_SCDG20_EID_RESIDENCE_PERMIT_2,
	CD_SCDG21_EID_OPTIONAL_DATA_RW,

	/// The result of validating the DG1 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG1_VALIDATE_EID,

	/// The result of validating the DG2 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG2_VALIDATE_EID,

	/// The result of validating the DG3 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG3_VALIDATE_EID,

	/// The result of validating the DG4 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG4_VALIDATE_EID,

	/// The result of validating the DG5 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG5_VALIDATE_EID,

	/// The result of validating the DG6 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG6_VALIDATE_EID,

	/// The result of validating the DG7 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG7_VALIDATE_EID,

	/// The result of validating the DG8 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG8_VALIDATE_EID,

	/// The result of validating the DG9 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG9_VALIDATE_EID,

	/// The result of validating the DG10 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG10_VALIDATE_EID,

	/// The result of validating the DG11 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG11_VALIDATE_EID,

	/// The result of validating the DG12 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG12_VALIDATE_EID,

	/// The result of validating the DG13 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG13_VALIDATE_EID,

	/// The result of validating the DG14 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG14_VALIDATE_EID,

	/// The result of validating the DG15 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG15_VALIDATE_EID,

	/// The result of validating the DG16 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG16_VALIDATE_EID,

	/// The result of validating the DG17 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG17_VALIDATE_EID,

	/// The result of validating the DG18 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG18_VALIDATE_EID,

	/// The result of validating the DG19 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG19_VALIDATE_EID,

	/// The result of validating the DG20 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG20_VALIDATE_EID,

	/// The result of validating the DG21 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG21_VALIDATE_EID,

	/// The result of validating the DG22 EID file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG22_VALIDATE_EID,

	/// The result of validating the hash of the e-Passport contents against the Signed
	/// Attributes from an RFID chip using the EF.CardSecurity.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCSIGNEDATTRS_VALIDATE_CARD_SECURITY_FILE,
	
	/// The result of validating the hash of the e-Passport contents against the Signed
	/// Attributes from an RFID chip using the EF.ChipSecurity.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCSIGNEDATTRS_VALIDATE_CHIP_SECURITY_FILE,
	
	/// The result of validating the signature in the EF.CardSecurity Document Security Object using a
	/// public key from the Document Signer Certificate (DSC) for an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item and cross-reference with DocSignerCertMode
	/// settings
	CD_SCSIGNATURE_VALIDATE_CARD_SECURITY_FILE,
	
	/// The result of validating the signature in the EF.ChipSecurity Document Security Object using a
	/// public key from the Document Signer Certificate (DSC) for an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item and cross-reference with DocSignerCertMode
	/// settings
	CD_SCSIGNATURE_VALIDATE_CHIP_SECURITY_FILE,
	
	/// The raw DG1 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG1_FILE_EID,
	
	/// The raw DG2 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG2_FILE_EID,
	
	/// The raw DG3 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG3_FILE_EID,
	
	/// The raw DG4 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG4_FILE_EID,
	
	/// The raw DG5 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG5_FILE_EID,
	
	/// The raw DG6 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG6_FILE_EID,
	
	/// The raw DG7 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG7_FILE_EID,
	
	/// The raw DG8 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG8_FILE_EID,
	
	/// The raw DG9 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG9_FILE_EID,
	
	/// The raw DG10 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG10_FILE_EID,
	
	/// The raw DG11 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG11_FILE_EID,
	
	/// The raw DG12 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG12_FILE_EID,
	
	/// The raw DG13 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG13_FILE_EID,
	
	/// The raw DG14 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG14_FILE_EID,
	
	/// The raw DG15 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG15_FILE_EID,
	
	/// The raw DG16 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG16_FILE_EID,	
		
	/// The raw DG17 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG17_FILE_EID,	
	
	/// The raw DG18 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG18_FILE_EID,	
	
	/// The raw DG19 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG19_FILE_EID,	
	
	/// The raw DG20 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG20_FILE_EID,	
	
	/// The raw DG21 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG21_FILE_EID,	

	/// The raw DG22 EID file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG22_FILE_EID,	

	/// The result of validating the EF.CardSecurity Document Signer Certificate's (DSC) signature using the
	/// public key from an external Country Signer Certificate (CSC) for an e-Passport.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item and cross-reference with ExternalCSCMode
	/// settings
	CD_VALIDATE_DOC_SIGNER_CERT_CARD_SECURITY_FILE,

	/// The result of validating the EF.ChipSecurity Document Signer Certificate's (DSC) signature using the
	/// public key from an external Country Signer Certificate (CSC) for an e-Passport.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item and cross-reference with ExternalCSCMode
	/// settings
	CD_VALIDATE_DOC_SIGNER_CERT_CHIP_SECURITY_FILE,

	/// \eac The EF.ChipSecurity file retrieved from an SAC enabled RFID chip, which contains 
	/// a list of SecurityInfos  to identify the algorithms to be used for SAC.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \remarks This file should only be present in e-Passports with SAC.
	CD_SCEF_CHIP_SECURITY_FILE,

	/// \eac The EF.CardSecurity file retrieved from an SAC enabled RFID chip, which contains 
	/// a list of SecurityInfos  to identify the algorithms to be used for SAC.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \remarks This file should only be present in e-Passports with SAC.
	CD_SCEF_CARD_SECURITY_FILE,

	/// The raw DG1 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG1_FILE_EDL,

	/// The raw DG2 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG2_FILE_EDL,

	/// The raw DG3 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG3_FILE_EDL,

	/// The raw DG4 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG4_FILE_EDL,

	/// The raw DG5 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG5_FILE_EDL,

	/// The raw DG6 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG6_FILE_EDL,

	/// The raw DG7 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG7_FILE_EDL,

	/// The raw DG8 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG8_FILE_EDL,

	/// The raw DG9 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG9_FILE_EDL,

	/// The raw DG10 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG10_FILE_EDL,

	/// The raw DG11 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG11_FILE_EDL,

	/// The raw DG12 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG12_FILE_EDL,

	/// The raw DG13 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG13_FILE_EDL,

	/// The raw DG14 EDL file data retrieved from an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; this represents the raw file in its
	/// binary format. \a aDataLen will represent the size of the data in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG14_FILE_EDL,

	/// The result of validating the DG1 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG1_VALIDATE_EDL,

	/// The result of validating the DG2 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG2_VALIDATE_EDL,

	/// The result of validating the DG3 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG3_VALIDATE_EDL,

	/// The result of validating the DG4 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG4_VALIDATE_EDL,

	/// The result of validating the DG5 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG5_VALIDATE_EDL,

	/// The result of validating the DG6 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG6_VALIDATE_EDL,

	/// The result of validating the DG7 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG7_VALIDATE_EDL,

	/// The result of validating the DG8 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG8_VALIDATE_EDL,

	/// The result of validating the DG9 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG9_VALIDATE_EDL,

	/// The result of validating the DG10 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG10_VALIDATE_EDL,

	/// The result of validating the DG11 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG11_VALIDATE_EDL,

	/// The result of validating the DG12 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG12_VALIDATE_EDL,

	/// The result of validating the DG13 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG13_VALIDATE_EDL,

	/// The result of validating the DG14 EDL file hash contained in the LDS Security Object
	/// of an RFID chip.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	/// \todo Document enabling data item
	CD_SCDG14_VALIDATE_EDL,
		
	/// \eac The decoded data retrieved from the DG1 data group of an RFID chip. This 
	/// represents the e-DL's license data.
	///
	/// \remarks Cast \a aDataPtr to a \c eDLDataGroup1Data*. \a aDataLen will represent the 
	/// size of the structure in bytes.
	CD_SCDG1_EDL_DATA,

	/// The decoded data retrieved from the DG2 data group of an RFID chip. This represents
	/// the e-DL photo.
	///
	/// \remarks Cast \a aDataPtr to a \c MMMByte*; the image is returned as an array of
	/// binary data. \a aDataLen will represent the size of the image in bytes.
	///
	/// \remarks The photo image will be in either the JPEG or JPEG 2000 format, depending
	/// on the e-Passport's country of origin.
	///
	/// \remarks The data is formatted such that it can be saved straight to disk if required.
	///
	/// \todo Document enabling data item
	CD_SCDG6_EDL_PHOTO,

	/// \eac The decoded data retrieved from the DG3 data group of an RFID chip. This 
	/// represents the e-DL's biometric fingerprint data.
	///
	/// \note This data item is only returned if Terminal Authentication is successful.
	///
	/// \remarks Cast \a aDataPtr to a \c DG3FingerprintData*. \a aDataLen will represent the 
	/// size of the structure in bytes.
	///
	/// \todo Document enabling data item
	CD_SCDG7_EDL_FINGERPRINTS,

	/// The comparison result between the datapage mrz and the chip mrz contained in the mrz datagroup.
	///
	/// \remarks The comparison result will be either True or false.
	CD_DATAPAGE_TO_CHIP_MRZ_COMPARISON,

	/// The comparison result between the datapage visible photo and the chip photo contained in the photo datagroup.
	///
	/// \remarks The comparison result will be either True or false.
	CD_DATAPAGE_TO_CHIP_FACE_COMPARISON,
		
	/// Represents the progress of the document detection.
	///
	/// \remarks Cast \a aDataPtr to a \c ReaderDocumentDetectionState*; this represents a single enumerated
	/// value. \a aDataLen will represent the size of a ReaderDocumentDetectionState enum in bytes.
	CD_DETECT_PROGRESS,

	/// The results from a lite data capture operation.
	///
	/// \remarks Cast \a aDataPtr to a \c LiteDataCapture *; this represents the results of
	/// a lite data capture operation performed.
	CD_DATA_CAPTURE_LITE,

	/// The results from a lite data capture operation.
	///
	/// \remarks Cast \a aDataPtr to a \c QaCodelineMeasurements *; this represents the results of
	/// a lite data capture QA Ocr operation performed.
	CD_DATA_CAPTURE_LITE_QAOCR,

	/// The results from a IDV remote document verification operation.
	///
	/// \remarks Cast \a aDataPtr to a \c char *; this represents the json results of
	/// a IDV remote operation performed.
	CD_IDV_REMOTE_DOCUMENT_VERIFICATION,

	/// The results from a IDV remote chip verification operation.
	///
	/// \remarks Cast \a aDataPtr to a \c char *; this represents the json results of
	/// a IDV remote operation performed.
	CD_IDV_REMOTE_CHIP_VERIFICATION,

	/// The results from a IDV remote face match operation.
	///
	/// \remarks Cast \a aDataPtr to a \c char *; this represents the json results of
	/// a IDV remote operation performed.
	CD_IDV_REMOTE_FACE_MATCH,

	/// The results from a text data extracted operation.
	///
	/// \remarks Cast \a aDataPtr to a \c TextDataExtracted *; this represents the results of
	/// a lite data capture operation performed.
	CD_TEXT_DATA_EXTRACTED,
		
	/// A decoded EU Digital Green Certificate (DGC) / Digital COVID Certificate (DCC)
	///
	/// \remarks Cast \a aDataPtr to a \c eDV_DigitalGreenCertificateData*; this represents the parsed data.
	CD_DIGITAL_GREEN_CERTIFICATE,

	/// The result of validating the signature of the EU Digital Green Certificate (DGC) using a
	/// public key from the Document Signer Certificate (DSC)
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	CD_DGC_SIGNATURE_VALIDATE,

	/// The result of validating the EU Digital Green Certificate's (DGC) Document Signer Certificate's 
	/// (DSC) signature using the public key from an external Country Signer Certificate (CSC)
	///
	/// \remarks Cast \a aDataPtr to a \c MMMReaderValidationCode*; this represents a single
	/// validation value. \a aDataLen will represent the size of #MMMReaderValidationCode in
	/// bytes. See #MMMReaderValidationCode for the possible values this can represent.
	///
	CD_DGC_DOC_SIGNER_CERT_VALIDATE,

	/// \internal
	NUM_MMMR_DATATYPES,

	/// The base data item identifier for all \sdk plugins that return data.
	///
	/// \remarks Do not use this value explicitly. All plugin data will be returned with
	/// a #MMMReaderDataType value of #CD_PLUGIN + #MMMReaderPluginData::puFeatureTypeID.
	/// See below for some pre-defined data item plugin constants.
	///
	/// \remarks For all data items based on #CD_PLUGIN, cast \a aDataPtr to a 
	/// \c MMMReaderPluginData*; this represents a single #MMMReaderPluginData structure.
	/// \a aDataLen will represent the size of the structure in bytes.
	CD_PLUGIN = 0x10000000,

	/// A 1D barcode encoded in the Industrial 2 of 5 symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_1D_INDUSTRIAL_2_OF_5		= CD_PLUGIN + 1,
	
	/// A 1D barcode encoded in the Interleaved 2 of 5 symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_1D_INTERLEAVED_2_OF_5	= CD_PLUGIN + 2,
	
	/// A 1D barcode encoded in the IATA 2 of 5 symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_1D_IATA_2_OF_5			= CD_PLUGIN + 3,
	
	/// A 1D barcode encoded in the Code 39 symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_1D_3_OF_9				= CD_PLUGIN + 4,
	
	/// A 1D barcode encoded in the Code 128 symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_1D_128					= CD_PLUGIN + 5,
	
	/// A 2D barcode encoded in the PDF417 symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_PDF417					= CD_PLUGIN + 6,
	
	/// \todo Is this still supported?
	CD_CHEQUES							= CD_PLUGIN + 7,
	
	/// A feature decoded from a UK driving licence.
	///
	/// \sa #CD_PLUGIN
	CD_UK_DRIVING_LICENCE				= CD_PLUGIN + 8, 
	
	/// \todo Is this still supported?
	CD_CHNVISAS							= CD_PLUGIN + 9,
	
	/// A 2D barcode encoded in the Aztec Code symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_AZTECCODE				= CD_PLUGIN + 10,
	
	/// \todo Is this still supported?
	CD_USVISAS  				 		= CD_PLUGIN + 11,
	
	/// A 2D barcode encoded in the QR Code symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_QRCODE					= CD_PLUGIN + 12,

	/// A 1D barcode encoded in the Code 93 symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_1D_CODE_93				= CD_PLUGIN + 13,

	/// A 1D barcode encoded in the Codabar symbology.
	///
	/// \sa #CD_PLUGIN
    CD_BARCODE_1D_CODABAR               = CD_PLUGIN + 14,

	/// A feature decoded from a document using the OCR Toolkit. Contact \ssd for more
	/// details.
	///
	/// \sa #CD_PLUGIN
	CD_OCRTOOLKIT  				 		= CD_PLUGIN + 15,

	/// A 1D barcode encoded in the UPC/EAN symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_1D_UPC_EAN              = CD_PLUGIN + 16,
	
	/// A 2D barcode encoded in the Data Matrix symbology.
	///
	/// \sa #CD_PLUGIN
	CD_BARCODE_DATAMATRIX				= CD_PLUGIN + 20,

} MMMReaderDataType_t;

/// mapping for some old names so that existing code still compiles
#define	CD_IMAGERESERVED1	CD_IMAGECOAXVIS
#define	CD_IMAGERESERVED2	CD_IMAGECOAXIR


/// Callback function definition for a user-defined function that will receive data from
/// the \hlapi.
///
/// \ingroup HighLevelDataModule
///
/// \note This callback is only relevant if using the \hlapi with the Non-Blocking API.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aDataType One of the #MMMReaderDataType values specifying which data item
/// the \hlapi is returning.
///
/// \param[in] aDataLen The length of the returned data, measured in bytes.
///
/// \param[in] aDataPtr A pointer to the returned data.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks \a aDataPtr is returned as a \c void*, so it must be cast to an appropriate type
/// before it can be used. See #MMMReaderDataType for an explanation of all available data items
/// and the appropriate type to cast the data to for each one.
///
/// \sa MMMReader_Initialise()
typedef void (*MMMReaderHLDataCallback)(
	void *aParam, 
	enum MMMReaderDataType aDataType,
	int aDataLen, 
	void *aDataPtr
);

/// Callback function definition for a user-defined function that provides feedback on document
/// positioning during document detection. This can be useful to inform the user so they can
/// better place the document on the \pagereader.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This callback is only relevant if using a \pagereader and the \hlapi
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aBoxCount The number of boxes in the \a aBoxes array. This value will have been
/// supplied from the MMMReader_InitialisePositionCorrection() API.
///
/// \param[in] aBoxes An array of Box structures. This value will have been supplied from the
/// MMMReader_InitialisePositionCorrection() API.
///
/// \param[in] aStates An array of boolean flags, one for each Box in \a aBoxes, stating
/// whether the area defined by the box is covered by the document.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \sa MMMReader_InitialisePositionCorrection()
typedef void (*MMMReaderPositionCorrectionCallback)(
	void *aParam, 
	int aBoxCount, 
	Box *aBoxes, 
	bool *aStates
);

/// Callback function definition for a user-defined function that is used during image decoding
/// via plugins. The defined function can be used to prevent a plugin from decoding an image
/// based on some user-defined criteria, e.g. if a previous plugin decode process has already
/// returned all the necessary data.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This callback is only relevant if using a \pagereader with the \hlapi.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is usually initialised
/// when setting the callback function in question.
///
/// \param[in] aPluginName A null-terminated string giving the name of the plugin that is
/// about to start the decode process.
///
/// \return Must return \c true if the host application wants to skip \a aPluginName, or
/// \c false if the host application wants \a aPluginName to continue.
///
/// \remarks \a aParam can be used to provide some context data back through the callback. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// function must be a C-style function, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks All plugin libraries used by the \sdk follow the file name convention 
/// <tt>RTDECODE_[name].dll</tt> where <tt>[name]</tt> will be the value that is returned via 
/// \a aPluginName.
///
/// \sa MMMReader_InitialiseSkipPlugin()
typedef bool (*MMMReaderHLSkipPluginCallback)(
	void *aParam,
	const RTCHAR *aPluginName
);

////////////////////////////////////////////// 
//////////////////// MMMReader High level API

/// Initialises the \sdk with the \hlapi.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aDataCallback A pointer to a user-defined function that will receive data
/// from the \sdk. This can be \c NULL; see the remarks on the Blocking API below.
///
/// \param[in] aEventCallback A pointer to a user-defined function that will receive event
/// triggers from the \sdk. This can be \c NULL; see the remarks on the Blocking API below.
///
/// \param[in] aErrorCallback A pointer to a user-defined function that will receive error
/// messages generated by the \sdk. This can be \c NULL.
///
/// \param[in] aCertCallback A pointer to a user-defined function that will be called when
/// an external certificate or private key is required for RF read operations. This can be
/// \c NULL; see the remarks below.
///
/// \param[in] aProcessMessages Specify whether operating system messages should be processed
/// while blocking, thus making applications responsive while blocking. This is only relevant
/// if using the Blocking API, otherwise it can be set to \c false.
///
/// \param[in] aProcessInputMessages Specify whether operating system messages should be 
/// processed while blocking, thus making applications responsive while blocking. This is only 
/// relevant if using the Blocking API, otherwise it can be set to \c false.
///
/// \param[in] aParam An optional pointer to some user-defined data. This is passed on to all
/// callbacks that have been set.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API must be the first API used before using any other function in the
/// \hlapi. The only exceptions to this rule are:
///		- MMMReader_EnableLogging()
///		- MMMReader_SetState()
///
/// \attention The MMMReader_Shutdown() API must be called when finished to free all resources
/// allocated.
///
/// \remarks \a aParam can be used to provide some context data back through the callbacks. One
/// example could be the \c this pointer from a C++ class; in this case, because the callback
/// functions must be C-style functions, \a aParam would be able to access elements of its
/// class type.
///
/// \remarks To use Blocking mode, \a aDataCallback and \a aEventCallback must be \c NULL.
/// To use Non-Blocking mode, either \a aDataCallback, \a aEventCallback or both <em>must</em> 
/// be provided.
///
/// \remarks During initialisation, all \sdk configuration settings will be loaded and this
/// will trigger the #SETTINGS_INITIALISED event (if \a aEventCallback is provided). This event
/// can be used to modify the loaded settings before they are committed. Once committed,
/// the \sdk will use those settings. For example:
///
/// \remarks \include HLSettingsInitialisedEvent.cpp
///
/// \remarks Similarly, once all plugins have been loaded and initialised, the 
/// #PLUGINS_INITIALISED event will be triggered.
///
/// \remarks If \a aCertCallback is provided, this may be used by the \sdk to get an external
/// certificate or private key. However, depending on the configuration of the \sdk, this may
/// never be needed; it is possible that certificates are loaded from another source, such as
/// a file store. See #EXTERNAL_CERT_MODE for more details.
///
/// \sa MMMReader_Shutdown(), MMMReader_IsInitialised()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_Initialise(
	MMMReaderHLDataCallback aDataCallback, 
	MMMReaderEventCallback aEventCallback,
	MMMReaderErrorCallback aErrorCallback,
	MMMReaderCertificateCallback aCertCallback,
	bool aProcessMessages,
	bool aProcessInputMessages,
	void *aParam
);


/// Shuts down the \pagereader and releases all resources allocated by the \sdk.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API must be called before the host application finishes, otherwise
/// memory leaks may occur.
///
/// \sa MMMReader_Initialise(), MMMReader_IsInitialised()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_Shutdown();

/// Shuts down and re-initialises the \pagereader and performs a full software reset of the
/// \sdk.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API can be used to attempt to recover from errors not detected by the
/// \sdk.
///
/// \remarks A full software reset is automatically triggered when in the #READER_ERRORED 
/// state.
///
/// \sa MMMReader_Initialise(), MMMReader_Shutdown()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_Reset();

/// Determine whether the \sdk has been fully initialised.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \return \c true if MMMReader_Initialise() has been called, or \c false otherwise.
///
/// \sa MMMReader_Initialise()
MMM_READER_HIGH_LEVEL_API bool MMMReader_IsInitialised();

/// Gets the current state of the \sdk and \pagereader.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \return A #ReaderState value specifying which state the \sdk and \pagereader is in.
///
/// \sa MMMReader_SetState()
MMM_READER_HIGH_LEVEL_API ReaderState MMMReader_GetState();

/// Sets the current state of the \sdk and \pagereader.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aNewState Specify the new #ReaderState the \sdk and \pagereader should be
/// set to.
///
/// \param[in] aForceRedetect Specify whether the \sdk should be forced to redetect a document.
/// See the remarks below.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks \a aNewState is restricted to only be one of the following:
///		- #READER_ENABLED
///		- #READER_DISABLED
///		- #READER_ASLEEP
///		- #READER_SUSPENDED
///
/// \remarks If \a aForceRedetect is \c true and a document is already on the \pagereader
/// window, it will be re-detected. A #READER_ENABLED state change will trigger the document
/// to be re-read.
///
/// \remarks If \a aForceRedetect is \c false and a document is already on the \pagereader
/// window, the document will not be read again until it is removed and replaced.
///
/// \remarks This API can be used to control the \pagereader during times when the host
/// application does not require document data or should not allow documents to be read.
/// For example, if an application requires that a document should be read during one
/// specific part of the application, the state could be set to #READER_DISABLED until the
/// application is ready to process document data, at which point the state could be set
/// to #READER_ENABLED.
///
/// \remarks This API may be used prior to MMMReader_Initialise() and can be used to control
/// the initial #ReaderState. For example, this is useful if the host application wishes to 
/// force the \pagereader to #READER_ASLEEP initially until the application is ready to accept 
/// data.
///
/// \sa MMMReader_GetState()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SetState(
	ReaderState aNewState, 
	bool aForceRedetect
);

/// Forces the \sdk to attempt to read a document, regardless of whether one is present
/// on the \pagereader window at the time.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Used in conjunction with the MMMReader_IsDocumentOnWindow() API, this API can
/// be used to re-read a document if it is known that it is still on the window.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_ForceRead();

/// \eac Set the #MMMReaderSignRequestCallback function to use for generating a digital
/// signature for the Terminal Authentication stage of e-Passport Extended Access Control 
/// (EAC).
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using the \hlapi and with an EAC-enabled build of the
/// \sdk. Contact \ssd if you require an EAC-enabled \sdk.
///
/// \note The \llapi has an equivalent API called MMMReader_RFSetSignRequestCallback().
///
/// \param[in] aSignRequestCallback A pointer to a user-defined function to perform the 
/// digital signing request.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API can only be used in an EAC-enabled build of the \sdk which has
/// the #RFProcessSettings::puEACEnabled setting set to \c true. If using this API when
/// this setting is \c false or the necessary features are unavailable, the error code
/// #ERROR_FEATURE_NOT_ENABLED will be returned.
///
/// \remarks The callback function provided will only be called if RF has been enabled.
///
/// \sa MMMReaderSignRequestCallback
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SetSignRequestCallback(
	MMMReaderSignRequestCallback aSignRequestCallback
);

MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SetRFPasswordCallback(
	MMMReaderRFPasswordCallback aCallback
);


/// Set the #MMMReaderWarningCallback function to use for generating information about
/// encoding warnings detected by the \sdk.
///
/// \ingroup HighLevelDocProcModule
///
/// \note The \llapi has an equivalent API called MMMReader_LL_SetWarningHandler().
///
/// \param[in] aCallback A pointer to a user-defined function to be informed of any warnings.
///
/// \note The \a aCallback will be called from within the scope of the thread that produced
/// the warning, so that thread will not continue until the warning has completed but other
/// threads may continue during this call. Therefore, callers should ensure that any
/// logic performed within the \a aCallback is thread-safe.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReaderWarningCallback
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SetWarningCallback(
	MMMReaderWarningCallback aCallback
);

/// Determines whether a document is currently placed on the \pagereader.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \return \c true if a document is present on the \pagereader window, or \c false otherwise.
///
/// \remarks This API is only relevant when the \sdk and \pagereader is in either the
/// #READER_ENABLED or #READER_DISABLED states.
MMM_READER_HIGH_LEVEL_API bool MMMReader_IsDocumentOnWindow();

/// Waits for a document to be placed on the \pagereader and will return when a document
/// has been detected.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using the \hlapi in \b Blocking mode.
///
/// \param[in] aTimeout Specify the maximum amount of time to wait for a new document in
/// milliseconds (ms).
///
/// \return 
///		- #NO_ERROR_OCCURRED if a new document was successfully detected.
///		- #ERROR_TIMED_OUT if a new document was not detected within the \a aTimeout specified.
///		- Otherwise, a #MMMReaderErrorCode stating what error occurred.
///
/// \remarks Once a document has been detected, use the MMMReader_ReadDocument() API to 
/// read it.
///
/// \sa MMMReader_IsDocumentOnWindow(), MMMReader_ReadDocument()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_WaitForDocumentOnWindow(
	int aTimeout
);

/// Reads a document placed on the \pagereader and stores all data items that have been
/// enabled via the \sdk configuration.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using the \hlapi in \b Blocking mode.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API will return only when all data has been read from the document.
///
/// \remarks If there is no document on the \pagereader window, this API will wait until a 
/// document has been detected. Because this API will continue to block further processing
/// until a document has been detected, it is advised that the 
/// MMMReader_WaitForDocumentOnWindow() API is used to determine whether it is suitable to 
/// read a document.
///
/// \remarks Once all data has been read, access it via the MMMReader_GetData() and 
/// MMMReader_GetPluginData() APIs.
///
/// \sa MMMReader_GetData(), MMMReader_GetPluginData(), MMMReader_WaitForDocumentOnWindow()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_ReadDocument();

/// Gets a particular data item read from the current document.
///
/// \ingroup HighLevelDataModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aDataType Specify which data item to retrieve. See #MMMReaderDataType for
/// an explanation of all available data items.
///
/// \param[out] aDataPtr A pointer which the \sdk will update to contain a local copy of the
/// data for \a aDataType. Can be \c NULL; see remarks for details.
///
/// \param[in,out] aDataLen Initially will contain the length of the pre-allocated 
/// \a aDataLen. When the function returns, it will update this value to be the actual size
/// of the data in bytes.
///
/// \param[in] aIndex Specify which item from a list of items to retrieve. To find out 
/// how many items are in a data types list call MMMReader_GetDataCount(). If aIndex
/// is set to an item that doesn't exist, aDataLen will be set to 0.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks In Blocking mode, this API should be called after MMMReader_ReadDocument().
/// In Non-Blocking mode, this API can be called at any point, but it will not wait for
/// data to be returned. In Non-Blocking mode, you would typically wait for data to be
/// returned via the #MMMReaderHLDataCallback.
///
/// \remarks If the data item is not present, \a aDataLen will be updated to be zero.
/// Possible reasons for this include:
///		- The \sdk has not been configured to read and process \a aDataType.
///		- In Non-Blocking mode, the data may not have been read/processed yet.
///
/// \remarks This API should be called twice; the first time, \a aDataPtr should be \c NULL
/// and \a aDataLen should be zero. If \a aDataType is present, the \sdk will then update
/// \a aDataLen to be the correct size for that data item. The second time the API is called,
/// \a aDataPtr should then point to a pre-allocated buffer of size \a aDataLen.
///
/// \remarks You can either call MMMReader_GetDataCount() to find out the valid values
/// for \a aIndex, or you can increment \a aIndex until \a aDataLen is set to 0.
///
/// \remarks This API should not be used for any #MMMReaderDataType after #CD_PLUGIN; use
/// MMMReader_GetPluginData() instead for retrieving data returned from the plugin decoders.
///
/// \sa MMMReader_GetDataCount(), MMMReader_GetPluginData()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetData(
	enum MMMReaderDataType aDataType,
	void *aDataPtr, 
	int *aDataLen,
	int aIndex
);

/// Gets the count of data items, of a particular data type, read from the current document.
///
/// \ingroup HighLevelDataModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aDataType Specify which data item to retrieve the count of. See #MMMReaderDataType for
/// an explanation of all available data items.
///
/// \param[out] aItemCount A pointer to an int which the \sdk will update to contain a 
/// the count of data items of type \a aDataType.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks In Blocking mode, this API can be called after MMMReader_ReadDocument().
/// The data returned can be used to know how many times to call MMMReader_GetData()
/// for each data item.
/// In Non-Blocking mode, this API can be called at any point, but it will not wait for
/// data to be returned. In Non-Blocking mode, you would typically wait for data to be
/// returned via the #MMMReaderHLDataCallback.
///
/// \remarks If the data item is not present, \a aItemCount will be updated to be zero.
/// Possible reasons for this include:
///		- The \sdk has not been configured to read and process \a aDataType.
///		- In Non-Blocking mode, the data may not have been read/processed yet.
///
/// \remarks This API should not be used for any #MMMReaderDataType after #CD_PLUGIN; use
/// MMMReader_GetPluginData() instead for retrieving data returned from the plugin decoders.
///
/// \sa MMMReader_GetData(), MMMReader_GetPluginData()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetDataCount(
	enum MMMReaderDataType aDataType,
	int *aItemCount
);

/// Gets data read from a \sdk plugin decoder from the current document.
///
/// \ingroup HighLevelDataModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aPluginName The name of the plugin to get the data from.
///
/// \param[in] aFeatureNum The feature number index to get.
///
/// \param[in] aPartNum The part number index to get.
///
/// \param[out] aDataPtr A pointer which the \sdk will update to return a pointer to a
/// MMMReaderPluginData structure containing the data.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks All plugin libraries used by the \sdk follow the file name convention 
/// <tt>RTDECODE_[name].dll</tt> where <tt>[name]</tt> is the name of the plugin. The name is
/// what should be passed in via \a aPluginName.
///
/// \remarks \a aFeatureNum and \a aPartNum are zero-based indexes used to identify which
/// feature and part to return, as a plugin can potentially return more than one item of
/// data per document read. See #MMMReaderPluginData for an explanation on plugin features and
/// parts.
///
/// \remarks If the requested \a aFeatureNum and/or \a aPartNum for \a aPluginName is not
/// present, \a aDataPtr will be updated to be \c NULL. Possible reasons for this include:
///		- \a aPluginName has not been enabled so no data will be returned from that plugin.
///		- The \a aFeatureNum index is greater than or equal to the number of features found.
///		- The \a aPartNum index is greater than or equal to the number of parts found for
///		\a aFeatureNum.
///
/// \remarks Often a plugin will simply find a single feature with a single part per document,
/// so in this case a single call to MMMReader_GetPluginData() will suffice, with 
/// \a aFeatureNum and \a aPartNum set to zero. Otherwise, a series of calls may be required.
/// For example, the code below demonstrates a generic way of getting all features and parts
/// for a plugin:
///
/// \remarks \include HLGetPluginData.cpp
///
/// \remarks This API should not be used for any #MMMReaderDataType below #CD_PLUGIN; use
/// MMMReader_GetData() instead for retrieving general data items.
///
/// \sa MMMReaderPluginData, MMMReader_GetData()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetPluginData(
	RTCHAR *aPluginName, 
	int aFeatureNum, 
	int aPartNum, 
	MMMReaderPluginData **aDataPtr
);

/// Clears all data that the \sdk has stored for the last document that was read.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Typically this API need not be called as the \sdk automatically clears all
/// data upon reading the next document. One possible use of this API is when memory
/// requirements are restricted and all data required from the last document read has been
/// obtained.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_ClearData();

/// Registers a callback function that provides feedback on document positioning during
/// document detection. This can be useful to inform the user so they can better place the
/// document on the \pagereader.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using a \pagereader and the \hlapi.
///
/// \param[in] aPositionCorrectionCallback A pointer to a user-defined function to receive
/// document position feedback.
///
/// \param[in] aBoxCount The number of boxes in the \a aBoxes array.
///
/// \param[in] aBoxes An array of Box structures which specify the regions of the \pagereader
/// window of interest.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks The \a aBoxes supplied will be checked during document detection to determine
/// whether a document is covering a Box or not. When the state of these \a aBoxes changes,
/// the \a aPositionCorrectionCallback will be called and supplied with \a aBoxCount, \a aBoxes
/// and an array of boolean flags stating which boxes are covered or not covered.
///
/// \remarks Position correction can be used by the host application to either display
/// this information to the user, or determine programatically the corrective action needed by
/// the user to ensure all the \a aBoxes are covered.
///
/// \remarks When using position correction, the \sdk will only notify that a document
/// is present when all \a aBoxes have been covered.
///
/// \sa MMMReaderPositionCorrectionCallback
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_InitialisePositionCorrection(
	MMMReaderPositionCorrectionCallback aPositionCorrectionCallback, 
	int aBoxCount, 
	Box aBoxes[]
);

/// Registers a callback function that is used during image decoding via plugins. The
/// callback function is used to prevent a plugin from decoding an image based on some
/// user-defined criteria, e.g. if a previous plugin decode process has already returned
/// all the necessary data.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param aSkipPluginCallback A pointer to a user-defined function to control which plugins
/// are used per document read.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReaderHLSkipPluginCallback
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_InitialiseSkipPlugin(
	MMMReaderHLSkipPluginCallback aSkipPluginCallback 
);

/// Aborts all RF chip reading operations whilst currently reading a document.
///
/// \ingroup HighLevelDocProcModule
///
/// \note This API is only relevant if using the \hlapi. The \llapi has an equivalent
/// API called MMMReader_LL_RFAbort().
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful in the case where the host application determines that it will
/// take too long to complete an RF read, and it would be quicker to abort the operation than
/// wait for all RF reading operations to fail one at a time.
///
/// \remarks This API only aborts RF reading; all other operations related to document
/// reading will continue as usual.
///
/// \remarks If there are no current RF operations waiting to be carried out or RF has 
/// been disabled, this function has no effect.
///
/// \remarks A typical scenario where this is useful is to abort an RF read if the document
/// has been removed from the \pagereader window, which can be detected via the #DOC_REMOVED
/// event. In order to carry out this scenario, the \sdk must be configured to carry out
/// document detection whilst reading the RF chip, accomplished by setting 
/// #DOCDETECTSETTINGS::detectDuringRF (from #MMMReaderSettings::puDocDetectSettings)
/// to \c TRUE.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_RFAbort();

/// Gets a local copy of the current \sdk settings.
///
/// \ingroup HLSettingsModule
///
/// \note This API is only relevant if using the \hlapi. The \llapi has an equivalent
/// API called MMMReader_LL_LoadSettings().
///
/// \param[out] aSettings A pointer to a MMMReaderSettings structure which the \sdk
/// will copy the current settings into.
///
/// \param[in,out] aSize Initially will contain the size of \a aSettings in bytes; this should
/// always be \c sizeof(MMMReaderSettings). When the function returns, this value will be the 
/// actual size of the MMMReaderSettings structure.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks If the initial value of \a aSize is incorrect, the error code
/// #ERROR_STRING_BUFFER_TOO_SMALL is returned. If \a aSize was set to 
/// \c sizeof(MMMReaderSettings) and this error occurs, it is in indication that the host
/// application is out-of-sync with the latest version of the \sdk. Recompile the host
/// application to correct this error.
///
/// \remarks This API will create a local copy of the current \sdk settings for the
/// host application to inspect. If the host application changes any of the settings stored
/// in the \a aSettings copy, those settings will not be committed until 
/// MMMReader_UpdateSettings() is called.
///
/// \remarks For an example of using MMMReader_GetSettings(), see the code sample provided
/// for MMMReader_Initialise().
///
/// \sa MMMReader_UpdateSettings(), MMMReader_SaveSettings()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetSettings(
	MMMReaderSettings *aSettings, 
	int *aSize
);

/// Gets the value of an \sdk setting
///
/// \ingroup HLSettingsModule
///
/// \note This API is only relevant if using the \hlapi. The \llapi has an equivalent
/// API called MMMReader_LL_GetSettingValue().
///
/// \param[out] aSectionName The name of the section the setting is located in.
///
/// \param[in] aSettingName The name of the setting.
///
/// \param[in/out] aSettingValue Pointer to memory where the value will be written.
/// It is assumed that the data type of the memory pointed to will be the correct
/// type for the setting requested.
///
/// \param[in/out] aSettingValueSize The size of the memory aSettingValue points to.
/// The value of this integer will be changed to specify the size of memory required
/// if aSettingValue is NULL or the size of memory used if aSettingValueSize was larger.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks If the initial value of \a aSettingValueSize is incorrect, the error code
/// #ERROR_DATA_BUFFER_TOO_SMALL is returned.
///
/// \sa MMMReader_GetSettings()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetSettingValue(
	const RTCHAR *aSectionName,
	const RTCHAR *aSettingName,
	void *aSettingValue,
	int *aSize
);

/// Commits any changes made to the \sdk settings for the current session.
///
/// \ingroup HLSettingsModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aNewSettings A pointer to a MMMReaderSettings structure which contains the
/// necessary changes to commit.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will commit any setting changes for the <em>current session</em>
/// only; once MMMReader_Shutdown() is called, all changes will be lost and 
/// MMMReader_Initialise() will load the settings from the \appconfig file again. To
/// commit setting changes to the \appconfig file, use the MMMReader_SaveSettings() API after
/// calling this API.
///
/// \remarks Typically, the host application will get a copy of the current settings using the
/// MMMReader_GetSettings() API, then change all necessary settings before calling this API to
/// commit all changes made. For an example of using MMMReader_GetSettings(), see the code 
/// sample provided for MMMReader_Initialise().
///
/// \remarks The most common settings to change are the values in
/// #MMMReaderSettings::puDataToSend, which controls the data items that are to be 
/// read/processed.
///
/// \remarks The \sdk and \pagereader will be temporarily placed into the #READER_ASLEEP state
/// while settings are being committed. Once completed, the previous #ReaderState will be
/// restored.
///
/// \remarks Some parts of the \sdk and/or \pagereader may get shutdown and re-initialised
/// depending on which settings have been changed. These include:
///		- RF module, which will get re-initialised if any of the #RFInitSettings or 
///		#RFProcessSettings in #MMMReaderSettings::puRFIDSettings were changed.
///		- Swipe Reader module, which will get re-initialised if the #SSSWIPE flag has been 
///		added to or removed from #MMMReaderSettings::puDataToSend.
///		- \aamva Parsing module, which will get re-initialised if the #SSAAMVA flag has been
///		added to or removed from #MMMReaderSettings::puDataToSend.
///		- UHF module, which will get re-initialised if the #SSUHF flag has been 
///		added to or removed from #MMMReaderSettings::puDataToSend.
///
/// \note If certain certificate settings in #RFProcessSettings are changed and a certificate
/// file store is used (#ECM_CERT_FILE_STORE), those certificate files may be reloaded. It is
/// important to remember this if the certificate file store is very large.
///
/// \sa MMMReader_GetSettings(), MMMReader_SaveSettings()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_UpdateSettings(
	MMMReaderSettings *aNewSettings
);

/// Commits the current \sdk settings for the current session to the \sdk configuration
/// file (\appconfig), thereby persisting the settings for future sessions to use.
///
/// \ingroup HLSettingsModule
///
/// \note This API is only relevant if using the \hlapi. The \llapi has an equivalent
/// API called MMMReader_LL_SaveSettings().
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention This API will overwrite the contents of the \appconfig file with the current 
/// \sdk settings. Ensure that this is what is required before using this API.
///
/// \remarks This API is only of use after calling MMMReader_UpdateSettings() and the
/// \sdk settings have been changed at some point in the current session. If no changes
/// have been made, this API has no effect.
///
/// \sa MMMReader_GetSettings(), MMMReader_UpdateSettings()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SaveSettings();

/// Writes a text file containing the members/values of the \sdk settings for the MMMReaderSettings param.
///
/// \ingroup HLSettingsModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aNewSettings A pointer to a MMMReaderSettings structure which contains the
///  data to be written to text file.
///
/// \param[in] aPathFileName Full path directory/filename to write the text data to.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_GetSettings(), MMMReader_UpdateSettings(), MMMReader_SaveSettings()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_WriteTextfileSettings(
	MMMReaderSettings *aNewSettings,
	char *aPathFileName
);

/// Enables or disables one of the \sdk plugins loaded.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aPluginName A null-terminated string giving the name of the plugin that should
/// be enabled/disabled.
///
/// \param[in] aEnabled Specify whether to enable or disable \a aPluginName.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks All plugin libraries used by the \sdk follow the file name convention 
/// <tt>RTDECODE_[name].dll</tt> where <tt>[name]</tt> will be the value that is passed in via 
/// \a aPluginName. The MMMReader_GetPluginName() API can be used to determine which
/// plugins are currently loaded.
///
/// \remarks If \a aPluginName is not present, #NO_ERROR_OCCURRED will be returned if
/// \a aEnabled is \c false. However, if attempting to enable a plugin which is not present
/// or errored on initialisation, this API will return an error.
///
/// \sa MMMReader_IsPluginEnabled(), MMMReader_GetPluginName()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_EnablePlugin(
	RTCHAR *aPluginName, 
	bool aEnabled
);

/// Determines if a particular \sdk plugin is enabled.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aPluginName A null-terminated string giving the name of the plugin to check.
///
/// \param[out] aEnabled A pointer to a boolean flag which will contain the result.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks All plugin libraries used by the \sdk follow the file name convention 
/// <tt>RTDECODE_[name].dll</tt> where <tt>[name]</tt> will be the value that is passed in via 
/// \a aPluginName. The MMMReader_GetPluginName() API can be used to determine which
/// plugins are currently loaded.
///
/// \sa MMMReader_EnablePlugin()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_IsPluginEnabled(
	RTCHAR *aPluginName, 
	bool *aEnabled
);

/// Gets the name of a \sdk plugin associated with an index value. Use this API to enumerate
/// through the list of currently loaded plugins.
///
/// \ingroup HighLevelDataModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[out] aPluginName A pointer to a buffer which will contain the name of the plugin
/// found. This can be \c NULL; see the remarks section below for more details.
///
/// \param[in,out] aPluginNameLen Initially will contain the length of the \a aPluginName 
/// buffer. When the function returns, this value will be the actual length of the plugin
/// name.
///
/// \param[in] aPluginIndex Specify the index to access in the internal \sdk list of
/// plugins. See the remarks section below for more details.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks To enumerate through all plugins, start \a aPluginIndex at zero and increase
/// this value until no plugin name is returned. This is indicated by both the 
/// \a aPluginNameLen being set to zero and \a aPluginName being updated to an empty string.
///
/// \remarks If the resulting plugin name is larger than the \a aPluginName buffer,
/// \a aPluginNameLen will be updated to be the correct length and this API will return with
/// the error code #ERROR_STRING_BUFFER_TOO_SMALL. Re-allocate the \a aPluginName buffer to
/// the correct size and call this API again to get the result.
///
/// \remarks The following example demonstrates how to use this API:
///
/// \remarks \include HLGetPluginName.cpp
///
/// \sa MMMReader_EnablePlugin(), MMMReader_IsPluginEnabled(), MMMReader_SetPluginOrder()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetPluginName(
	RTCHAR *aPluginName, 
	int *aPluginNameLen, 
	int aPluginIndex
);

/// Specify the order that the \sdk plugins should be processed in. Use this API to elevate
/// a particular plugin over another.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aPluginName A null-terminated string giving the name of the plugin to re-order.
///
/// \param[in] aOrder Specify the relative processing order of the plugin in relation to
/// the other plugins.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks All plugin libraries used by the \sdk follow the file name convention 
/// <tt>RTDECODE_[name].dll</tt> where <tt>[name]</tt> will be the value that is passed in via 
/// \a aPluginName. The MMMReader_GetPluginName() API can be used to determine which
/// plugins are currently loaded.
///
/// \remarks Used in conjuction with the MMMReader_InitialiseSkipPlugin() API, it is possible
/// to speed up plugin decoding if the host application has certain knowledge on what 
/// plugin-related data it should expect.
///
/// \sa MMMReader_GetPluginName()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SetPluginOrder(
	RTCHAR *aPluginName, 
	int aOrder
);


/// Gets a list of serial numbers of all \pagereaders that are currently connected to the
/// host machine.
///
/// \ingroup HighLevelDataModule
///
/// \note This API is only relevant if using multiple \pagereaders with the \hlapi. The 
/// \llapi has an equivalent API called MMMReader_LL_GetConnectedScanners().
///
/// \param[out] aSerialNumbers A pointer to a string buffer which will contain the list
/// of serial numbers found. This can be \c NULL; see the remarks section for more details.
///
/// \param[in,out] aSerialNumbersLen Initially will contain the length of the \a aSerialNumbers 
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \param[out] aNumScanners A pointer to an integer which will return how many \pagereaders
/// are currently connected to the host machine.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aSerialNumbers should be
/// \c NULL and \a aSerialNumbersLen should be zero. This will provide the correct length
/// of the \a aSerialNumbers buffer via \a aSerialNumbersLen. The second time should then
/// pass a correctly allocated string buffer to contain the results.
///
/// \remarks The serial numbers returned via \a aSerialNumbers are separated by the semi-colon
/// (\c ";") character.
///
/// \remarks If only one \pagereader is connected, then only one serial number is returned
/// via \a aSerialNumbers.
///
/// \remarks If more than one \pagereader is connected, then this API should be used in
/// conjunction with MMMReader_SelectScanner() to attach to the appropriate \pagereader.
///
/// \sa MMMReader_SelectScanner()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetConnectedScanners(
	char *aSerialNumbers, 
	int aSerialNumbersLen, 
	int *aNumScanners
);

/// Attaches to the indicated \pagereader via its serial number.
///
/// \ingroup HighLevelInitStateModule
///
/// \note This API is only relevant if using multiple \pagereaders with the \hlapi. The 
/// \llapi has an equivalent API called MMMReader_LL_SelectScanner().
///
/// \param[in] aSerialNumber A null-terminated string specifying the serial number of the
/// \pagereader to attach to.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API allows the host application to operate correctly when there are multiple
/// \pagereaders connected to the same host machine. If there is only one \pagereader connected,
/// then there is no need to call this API.
///
/// \remarks The \a aSerialNumber used is retrieved from the MMMReader_GetConnectedScanners()
/// API.
///
/// \note Special configurations may be required to operate multiple \pagereaders from
/// one host application. Customers wishing to use the \sdk in this way should contact \ssd for
/// assistance.
///
/// \note This API does not allow the use of multiple \pagereaders at the same time within
/// the same host application; one \pagereader must always be selected at any one time. However
/// it is possible for two host applications/instances to control one \pagereader via this
/// API.
///
/// \sa MMMReader_GetConnectedScanners()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SelectScanner(
	const char *aSerialNumber
);

/// Enables or disables the \sdk logging feature.
///
/// \ingroup HLErrorsLoggingModule
///
/// \note This API is only relevant if using the \hlapi. The \llapi has an equivalent API 
/// called MMMReader_InitialiseLogging().
///
/// \param[in] aEnabled Specify whether logging should be enabled or disabled.
///
/// \param[in] aLogLevel Specify how detailed the logging should be. If \a aEnabled is 
/// \c false, this value can be ignored (set to -1).
///
/// \param[in] aLogMask Specify which areas of the \sdk should be logged to file by providing
/// a bitmask of flags. Typically, this value should be #LOGMASK_ALL.
///
/// \param[in] aFilename Specify the name of the file to write log messages to. If 
/// \a aEnabled is \c false, this value can be ignored (set to \c NULL).
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful for generating log files for diagnosing problems; \ssd may
/// ask you to enable logging for such a scenario.
///
/// \remarks Altering the \a aLogLevel will produce more or less detailed log files. Zero (0)
/// represents logging of errors only, while 5 represents the highest, most detailed logging
/// possible. \ssd will typically ask for log level 5 files to assist them in diagnosing 
/// problems.
///
/// \remarks Once logging has been enabled, the host application may use the 
/// MMMReader_LogMessage() and MMMReader_LogFormatted() APIs to add its own messages into the
/// main \sdk log file.
///
/// \sa MMMReader_LogMessage(), MMMReader_LogFormatted(), MMMReader_GetLastError()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_EnableLogging(
	bool aEnabled, 
	int aLogLevel, 
	int aLogMask, 
	RTCHAR *aFilename
);

/// Gets the error string and error code associated with the last \sdk error that occurred.
///
/// \ingroup HLErrorsLoggingModule
///
/// \note This API is only relevant if using the \hlapi. The \llapi has an equivalent API 
/// called MMMReader_LL_GetLastError().
///
/// \param[out] aErrorCode A pointer to a #MMMReaderErrorCode which will contain the last
/// error code recorded.
///
/// \param[out] aErrorString A pointer to a string buffer which will contain the last error
/// string recorded. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aStrLen Initially will contain the length of the \a aErrorString 
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aErrorString should be
/// \c NULL and \a aStrLen should be zero. This will provide the correct length of the 
/// \a aErrorString buffer via \a aStrLen. The second time should then pass a correctly 
/// allocated string buffer to contain the result.
///
/// \remarks This API will only return the last error recorded. If an #MMMReaderErrorCallback
/// was supplied to MMMReader_Initialise(), all errors will also be received via that
/// callback function.
///
/// \sa MMMReader_GetErrorMessage()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetLastError(
	MMMReaderErrorCode *aErrorCode, 
	RTCHAR *aErrorString, 
	int *aStrLen
);

/// Logs a message from the host application to the main \sdk log file.
///
/// \ingroup HLErrorsLoggingModule
///
/// \note This API is only relevant if using the \hlapi when logging has been enabled. The 
/// \llapi has an equivalent API called MMMReader_LL_LogMessage().
///
/// \param[in] aLevel Specify the level of the log message. The higher the number, the less
/// important the message is.
///
/// \param[in] aMask Typically for host applications, this should be #LOGMASK_ALL.
///
/// \param[in] aLocation A null-terminated string specifying the location where the message
/// was generated. Typically this is the code location in the host application where the 
/// message was generated to assist in tracing messages in the log file.
///
/// \param[in] aMessage A null-terminated string providing the message to write to the log
/// file.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful for host applications to add their own messages to the main
/// \sdk log file. It is recommended that this API be only used to add messages related to the
/// \pagereader and/or \sdk, and not general application activity.
///
/// \remarks If logging has been disabled via the MMMReader_EnableLogging() API, this API
/// will have no effect.
///
/// \remarks Only messages that have a \a aLogLevel less than the value provided to 
/// MMMReader_EnableLogging() will be written to the \sdk log file.
///
/// \sa MMMReader_EnableLogging(), MMMReader_LogFormatted()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_LogMessage(
	int aLevel, 
	int aMask, 
	RTCHAR *aLocation, 
	RTCHAR *aMessage
);

/// Logs a formatted message from the host application to the main \sdk log file.
///
/// \ingroup HLErrorsLoggingModule
///
/// \note This API is only relevant if using the \hlapi when logging has been enabled. The 
/// \llapi has an equivalent API called MMMReader_LL_LogFormatted().
///
/// \param[in] aLevel Specify the level of the log message. The higher the number, the less
/// important the message is.
///
/// \param[in] aMask Typically for host applications, this should be #LOGMASK_ALL.
///
/// \param[in] aLocation A null-terminated string specifying the location where the message
/// was generated. Typically this is the code location in the host application where the 
/// message was generated to assist in tracing messages in the log file.
///
/// \param[in] aFormat A null-terminated string providing the format of the message to write 
/// to the log file. This can contain formatting flags as used in C functions such as
/// \c printf(), \c sprintf(), etc.
///
/// \param[in] ... A variable list of parameters which match up with the formatting flags
/// in \a aFormat to create the literal message.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API is useful for host applications to add their own messages to the main
/// \sdk log file. It is recommended that this API be only used to add messages related to the
/// \pagereader and/or \sdk, and not general application activity.
///
/// \remarks The difference between this API and MMMReader_LogMessage() is that this API
/// acts in a similar way to the C functions such as \c printf(), \c sprintf(), etc.
/// Formatting flags (e.g. \c "%d", \c "%s", etc.) can be provided in \a aFormat, where they
/// will be replaced by the variable list of parameters after \a aFormat. This API is useful
/// when the host application needs to log non-string values to the log file.
///
/// \remarks If logging has been disabled via the MMMReader_EnableLogging() API, this API
/// will have no effect.
///
/// \remarks Only messages that have a \a aLogLevel less than the value provided to 
/// MMMReader_EnableLogging() will be written to the \sdk log file.
///
/// \sa MMMReader_EnableLogging(), MMMReader_LogMessage()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_LogFormatted(
	int aLevel, 
	int aMask, 
	RTCHAR *aLocation, 
	RTCHAR *aFormat, 
	...
);

/// Gets the error string associated with a given #MMMReaderErrorCode.
///
/// \ingroup HLErrorsLoggingModule
///
/// \note This API is only relevant if using the \hlapi. The \llapi has an equivalent API 
/// called MMMReader_LL_GetErrorMessage().
///
/// \param[in] aErrorCode The #MMMReaderErrorCode to get the error message for.
///
/// \param[out] aErrorString A pointer to a string buffer which will contain the error
/// string. This can be \c NULL; see the remarks section below for details.
///
/// \param[in,out] aStrLen Initially will contain the length of the \a aErrorString 
/// buffer. When the function returns, this value will be the actual length of the string.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This API should be called twice; the first time, \a aErrorString should be
/// \c NULL and \a aStrLen should be zero. This will provide the correct length of the 
/// \a aErrorString buffer via \a aStrLen. The second time should then pass a correctly 
/// allocated string buffer to contain the result.
///
/// \note The error string returned may include formatting flags (e.g. \c "%d", \c "%s", etc.)
/// which would typically be populated with context dependent data when \a aErrorCode
/// occurs.
///
/// \sa MMMReader_GetLastError()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetErrorMessage(
	MMMReaderErrorCode aErrorCode, 
	RTCHAR *aErrorString, 
	int *aStrLen
);

/// Gets a \llapi function pointer, allowing the host application to perform operations
/// typically related only to the \llapi, or not provided via the \hlapi.
///
/// \ingroup HighLevelTunnelModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aFuncName A null-terminated string to the name of the \llapi function
/// to get.
///
/// \param[out] aFuncPtr A pointer to the \llapi function pointer.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \attention Generally, due to the queueing nature of the non-blocking \llapi calls, it is 
/// usually safe to call a \llapi function while the \hlapi is currently reading a document. 
/// However, great care must be taken if calling the \llapi from the \hlapi. Contact \ssd for 
/// assistance if required.
///
/// \remarks \a aFuncPtr must be cast to a suitable function pointer definition in order to
/// be usable.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetLowLevelAPI(
	const RTCHAR *aFuncName, 
	void **aFuncPtr
);

/// Updates the aDocPosition parameter to contain the location of the currently detected document.
/// Typically this is useful when you are tunnelling into the low level API, and need to have the
/// document position for other APIs (such as reading the codeline, or cropping images).
///
/// \ingroup HighLevelTunnelModule
///
/// \note This API is only relevant if using the \hlapi.
///
/// \param[in] aDocPosition A pointer to the Box that should be updated
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// 
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetDocumentPosition(Box *aDocPosition);


//#ifdef DCA
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_Initialise_Ex(
	MMMReaderHLDataCallback aDataCallback, 
	MMMReaderEventCallback aEventCallback,
	MMMReaderErrorCallback aErrorCallback,
	MMMReaderCertificateCallback aCertCallback,
	MMMReaderMrzCorrectionCallback aMrzCorrectionCallback,
	bool aProcessMessages,
	bool aProcessInputMessages,
	void *aParam
);

/// Sets the licence key file, which determines what \sdk functionality can be used.
///
/// \note The \llapi has an equivalent API called MMMReader_LL_SetLicenseKeyfile().
///
/// \param [in] aFilename The location of the licence key file.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SetLicenseKeyfile(
	char *aFilename
);

/// Get a list of permission flags for all the various functions of \sdk.
///
/// \note The \llapi has an equivalent API called MMMReader_LL_GetLicensePermissions.
///
/// \param [out] aPermissionList A pointer to a PermissionList structure, that contains 
/// flags that will be set to specify whether the current licence grants permission for
/// given \sdk funtions.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \todo Document what each element of the array corrisponds to.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetLicensePermissions(
	PermissionList *aPermissionList
);

/// Retrieve the data for a given data item.
///
/// \param [out] aDataFormat The format of the data returned in aDataPtr. 
/// Data items that are images will be returned as encoded bmp.
///
/// \param [out] aLocationInfo Details about where the data came from, such as 
/// the location of the data on the image and the image type.
///
/// \param [in] aProcessName The name of the process to which the data item belongs. This can be NULL.
///
/// \param [in] aDataItemName The name of the data item to retrieve.
///
/// \param [in/out] aDataPtr A pointer to memory where the value will be written.
/// It is assumed that the data type of the memory pointed to will be the correct
/// type for the data requested.
///
/// \param[in/out] aDataLen The size of the memory aDataPtr points to.
/// The value of this integer will be changed to specify the size of memory required
/// if aDataLen is NULL or the size of memory used if aDataLen was larger.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetItemData(
	MMMReaderDataFormat *aDataFormat, 
	LocateInfo *aLocationInfo,
	char *aProcessName,
	char *aDataItemName,
	void **aDataPtr, 
	int *aDataLen
);

/// Set the data for a given data item.
///
/// \param [in] aDataFormat The format of aBuffer.  Data items that are images will be decoded as bmp.
///
/// \param [in] aProcessName The name of the process to which the data item belongs. This can be NULL.
///
/// \param [in] aDataItemName The name of the data item to set.
///
/// \param [in] aBuffer A pointer to memory where the value will be copied from.
/// It is assumed that the data type of the memory pointed to will be the correct
/// type for the data item being set.
///
/// \param[in] aBuffer The size of the memory aBuffer points to in bytes.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SetItemData(
	MMMReaderDataFormat aDataFormat, 
	char *aProcessName,
	char *aDataItemName, 
	void *aBuffer, 
	int aBufferSize
);

/// Load data saved from a previous scan.
///
/// \param [in] aDirectoryPath The location of the data to load.
///
/// \param [in] aSendDataFlag indicates if the calling function wants the corresponding CD_ data event 
///  to also be dispatched.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_LoadFromScanDirectory(
	char *aDirectoryPath,
	bool aSendDataFlag
);

/// Initiate the processing of data that has been manually loaded.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_SetItemData(), MMMReader_LoadFromScanDirectory()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_ProcessData(
);

/// Specifies the next document to be processed.
///
/// \param [in] aGroupName The name of the document group.
///
/// \param [in] aSpecificName The name of the document type.
///
/// \param [in] aVariantName The name of the variant of document to use.
///
/// \param [in] aFlipSide If true the rear of document is used if false the front is used.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks This function can be used for speeding up the identification process if the type
/// of document is known.
///
/// \remarks For problmatic documents, or documents that it is impossible to idetify,
/// this function will have to be used.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SetNextDocumentExpected(
	char *aGroupName,
	char *aSpecificName,
	char *aVariantName,
	bool aFlipSide
);

/// Utility function to produce a formmated date string.
///
/// \param [in] aDate The date as a MMMReaderDate struct pointer.
///
/// \param [in] aFormat The name of the document type.
///
/// \param [in/out] aOutDateStr A pointer to memory where the formatted date will be written.
///
/// \param[in/out] aOutDateStrSize The size of the memory aOutDateStr points to.
/// The value of this integer will be changed to specify the size of memory required
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_FormatDate(
	MMMReaderDate *aDate,
	const RTCHAR *aFormat,
	RTCHAR *aOutDateStr,
	int *aOutDateStrSize
);


/// Instructs the sdk high level code to reset any flags controlling multi side document processing to initial (first side) state.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_ResetSideState();


/// Retrieves the Layout Directory path as cimpiled frm the reader.ini file.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_GetLayoutDirectory(
	RTCHAR *aLayoutDirectoryString, 
	int *aStrLen
);


//#endif DCA

/// Load data saved from a previous scan and ocr process the images.
///
/// \param [in] aDirectoryPath The location of the data to load.
///
/// \param [in] aSendDataFlag indicates if the calling function wants the corresponding CD_ events 
///  to also be dispatched.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_LoadAndProcessFromScanDirectory(
	const char *aDirectoryPath,
	bool aSendDataFlag
);

/// Capture the desired image utilizing the HighLevel image settings.  The reader state 
///    must be READER_ASLEEP otherwise the reader document detection will block any image capture calls.
///
/// \param [in] alight The image illumination type.
///
/// \param [in] aImageFormat The output image data format.
///
/// \param [in] aCompression The output image compression level for JPEG image format.
///
/// \param [in] aCropWithExistingLocate The output image will be cropped with information gathered from the last document processed.
///
/// \param [in] aRaw The output image will be the raw camera image without any post-processing or cropping.
///
/// \param [in] aAntiGlare Turn AntiGlare on/off when capturing image.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.

MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_CaptureImage(
	enum MMMReaderDataType alight,
	MMMReaderImageFormats aImageFormat, 
	int aCompression,
	bool aCropWithExistingLocate,
	bool aRaw,
	bool aAntiGlare
);

/// Is image cropping enabled for the related data type.  This function must be called after
/// the SDK is initialized.  The module may not be ready until the initial reader state has been set.
///
/// \param [in] aDataType The data type associated with the image type (eg CD_IMAGEVIS).
///
/// \param [out] aEnabled A pointer to a boolean flag which will contain the result. 
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_IsImageCroppingEnabled(
	enum MMMReaderDataType aDataType,
	bool *aEnabled
);

/// Set image cropping enabled for the related data type.  This function must be called after
/// the SDK is initialized.  The module may not be ready until the initial reader state has been set.
///
/// \param [in] aDataType The data type associated with the image type (eg CD_IMAGEVIS).
///
/// \param [in] aEnabled Specify whether to enable or disable image cropping.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_EnableImageCropping(
	enum MMMReaderDataType aDataType,
	bool aEnabled
);

/// Signal an event has occurred with potentially a sound and the LED indicators.
///
/// \param[in] aSoundSettings A pointer to a SoundSettings structure which should
/// contain the required settings for the \pagereader.
///
/// \param[in] aAssumeControl Specify the calling application will assume complete control of the leds and sound signalling from the HL api.
///
/// \param[in] aEvent Specify which #MMMReaderSignal to indicate.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \remarks Typically, the required settings structures are taken from #MMMReaderSettings by
/// using MMMReader_GetSettings().
///
/// \remarks Some of the #MMMReaderSignal values can only be used for either signalling the
/// LEDs or signalling a sound, while most can be used for both.
///
/// \sa MMMReader_LedEvent(), MMMReader_SoundEvent()
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_SignalEventEx(
	bool aAssumeControl,
	SoundSettings *aSoundSettings, 
	LedSettings *aLedSettings, 
	MMMReaderSignal aEvent
);


/// Helper function to extract useful information from IDV Remote Json responses.
///
/// \param[in] aResponseType A int value to indicate the source response type 1 - DocumentVerify, 2 - ChipVerify (not implemented), 3 - FaceMatch
///
/// \param[in] aResponseJson The char string json response.
///
/// \param[in] aItemNames Pointer to an array of char pointers that will be turned.  These will contain display name for the item value list
///
/// \param[in] aItemValues Pointer to an array of char pointers that will be turned. These will contain item values found/extracted.
///
/// \param[in] aItemCount Pointer to an int containing the number of items in the above arrays..
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_DecodeIDVRemoteJson(int aResponseType, char *aResponseJson, char **aItemNames[], char **aItemValues[], int *aItemCount);


/// ScanForBarcode simulates a dedicated barcode scanner with a document reader.
/// The method performs multiple captures as fast as possible, while decoding the images and returns
/// when a barcode is found.
/// 
/// The High Level API must be initialized and in a READER_ASLEEP or READER_DISABLED state.
///
/// \param [in] aTimeout Timeout (Milliseconds) of how long to wait for a barcode.
///
/// \param [in] aWaitForDocument Enable or Disable waiting for a document before scanning. The settings of the High Level API
///             are used, such as DarkObjects enabled, for cell phone detection.
///             NOTE: if aWaitForDocument is FALSE, set aIsDarkObjectOverride = TRUE and pass aIsDarkObject = TRUE or FALSE
///             for cellphone or paper. Otherwise, the last known detection state of the High Level API is used.
///
/// \param [in] aSendData True to send barcode data found to the High Level API Data Callback. If False, use MMMReader_GetPluginData after the call.
///
/// \param [in] aIsDarkObjectOverride True to use aIsDarkObject as the type of document. Otherwise, the last known detection state of the High Level API is used.
///             NOTE: only used if aWaitForDocument is FALSE.
///
/// \param [in] aIsDarkObject True if the document is a cell phone and using aIsDarkObjectOverride.
///             NOTE: only used if aWaitForDocument is FALSE.
///
/// \param [in] aUseBinnedCapture Use binned image for faster capture on high resolution readers, such as the AT10K 550.
///
/// \param [in/out] aQRCode [in] Set precedence of decoder 0 = OFF.  [out] Upon return, set to the number of barcodes found for this decoder.
///
/// \param [in/out] aDataMatrix Set precedence of decoder 0 = OFF.  [out] Upon return, set to the number of barcodes found for this decoder.
///
/// \param [in/out] aPDF417 Set precedence of decoder 0 = OFF.  [out] Upon return, set to the number of barcodes found for this decoder.
///
/// \param [in/out] aAztec Set precedence of decoder 0 = OFF.  [out] Upon return, set to the number of barcodes found for this decoder.
///
/// \param [in/out] aOneD Set precedence of decoder 0 = OFF.  [out] Upon return, set to the number of barcodes found for this decoder.
///
/// \param [in/out] aBarcodesFound The number of barcodes found.
///
/// \return #NO_ERROR_OCCURRED if successful, otherwise a #MMMReaderErrorCode stating what
/// error occurred.
///
/// \sa MMMReader_ScanForBarcodeAbort()
/// 
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_ScanForBarcode(
	int aTimeout,
	bool aWaitForDocument,
	bool aSendData,
	bool aIsDarkObjectOverride,
	bool aIsDarkObject,
	bool aUseBinnedCapture,
	bool aFindMultiple,
	int* aQRCode,
	int* aDataMatrix,
	int* aPDF417,
	int* aAztec,
	int* aOneD,
	int* aBarcodesFound
);

/// Aborts a currently running ScanForBarcode process.
/// 
MMM_READER_HIGH_LEVEL_API MMMReaderErrorCode MMMReader_ScanForBarcodeAbort();


#ifdef __cplusplus
 } 
#endif //__cplusplus

#endif //_MMMREADERHIGHLEVELAPI_H_
