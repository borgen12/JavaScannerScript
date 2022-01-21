package com.reader;

import com.mmm.readers.*;
import com.mmm.readers.FullPage.*;
import com.mmm.readers.interop.Marshal;
import com.mmm.readers.modules.Swipe.MsrData;
import com.mmm.readers.modules.rfid.CertificateHandler;
import com.mmm.readers.modules.rfid.CertificateObject;
import com.mmm.readers.modules.rfid.CertificateType;
import com.socket.SocketClient;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class KioskScannerNon implements DataHandler, ErrorHandler, EventHandler, CertificateHandler {
    private Reader prFullPageReader;
    private boolean prInitialised;
    private int prDetectState;
    private File prCurrentCertDir;
    private int prDetectStateCounter;
    private SocketClient socket;

    public void kioskScannerNon() throws IOException {
        try{
            //this.startSocket();
            this.initialiseScanner();
            this.prInitialised = false;
            this.prDetectState = 0;
            this.prDetectStateCounter = 0;
        }
        catch (Throwable e) {
            throw (e);
        }
    }

    public void startSocket() throws IOException {
        socket = new SocketClient();
        socket.startConnection();
    }

    private void initialiseScanner() {
        try {
            System.out.println("Loading Highlevel dll...");
            this.prFullPageReader = new Reader();
            System.out.println("Highlevel dll loaded");
            this.prFullPageReader.EnableLogging(true, 5, -1, "NonBlockingJava.log");
            System.out.println("Initialising...");
            ErrorCode var2 = this.prFullPageReader.Initialise(this, this, this, this, true, false, 0);
            if (var2 != ErrorCode.NO_ERROR_OCCURRED) {
                if (var2 == ErrorCode.ERROR_MISMATCH_IN_AN_ENUM) {
                    System.out.println("ERROR: Mismatch in an Enum");
                } else if (var2 == ErrorCode.ERROR_MISMATCH_SETTING_SIZE) {
                    System.out.println("ERROR: Mismatch in the Settings");
                } else {
                    System.out.println("Initialise error: " + var2.toString());
                }
            } else {
                System.out.println("Initialise successful");
                this.prInitialised = true;
            }
        } catch (Throwable var3) {
            System.out.println("Unable to initialise " + var3.toString());
            var3.printStackTrace();
        }

    }

    private void formWindowClosing(WindowEvent var1) {
        if (this.prInitialised) {
            this.shutdownReader();
        }
    }

    public static void main(String[] var0) throws IOException{
        EventQueue.invokeLater(new Runnable() {
            public void run() {
                try {
                    KioskScannerNon scanner = new KioskScannerNon();
                    scanner.kioskScannerNon();
                }
                catch(Throwable e) {
                }
            }
        });
       /* KioskScannerNon scanner = new KioskScannerNon();
        scanner.kioskScannerNon();*/
    }

    private void shutdownReader() {
        ErrorCode var1 = this.prFullPageReader.Shutdown();
        if (var1 == ErrorCode.NO_ERROR_OCCURRED) {
            System.out.println("Shutdown successful");
        }
    }

    public void OnFullPageReaderData(DataType var1, int var2, byte[] var3) {
        boolean var4 = false;
        String var8;
        int var15;
        PluginData var23;

        switch(var1) {
            case CD_CODELINE:
            case CD_SCDG1_CODELINE:
            case CD_SCCHIPID:
            case CD_SCAIRBAUD:
                String var17 = new String(var3, 0, var2 - 1);
                System.out.println("Data: " + var1.toString() + " = " + var17);
                break;
            case CD_CODELINE_DATA:
            case CD_SCDG1_CODELINE_DATA:
                CodelineData var16 = this.prFullPageReader.ConstructCodelineData(var3);
                //this.AddCodelineDataToMsgList(var16);
                break;
            case CD_IMAGEIR:
            case CD_IMAGEVIS:
            case CD_IMAGEUV:
            case CD_IMAGEPHOTO:
            case CD_IMAGEBARCODE:
            case CD_SCDG2_PHOTO:
                System.out.println("Data: " + var1.toString() + " = <image - " + var2 + " bytes>");
                break;
            case CD_SCDG1_FILE:
            case CD_SCDG2_FILE:
            case CD_SCDG3_FILE:
            case CD_SCDG4_FILE:
            case CD_SCDG5_FILE:
            case CD_SCDG6_FILE:
            case CD_SCDG7_FILE:
            case CD_SCDG8_FILE:
            case CD_SCDG9_FILE:
            case CD_SCDG10_FILE:
            case CD_SCDG11_FILE:
            case CD_SCDG12_FILE:
            case CD_SCDG13_FILE:
            case CD_SCDG14_FILE:
            case CD_SCDG15_FILE:
            case CD_SCDG16_FILE:
            case CD_SCEF_COM_FILE:
            case CD_SCEF_SOD_FILE:
                System.out.println("Data:  " + var1.toString() + " = <binary data - " + var2 + " bytes>");
                break;
            case CD_SECURITYCHECK:
            case CD_SCDG1_VALIDATE:
            case CD_SCDG2_VALIDATE:
            case CD_SCDG3_VALIDATE:
            case CD_SCDG4_VALIDATE:
            case CD_SCDG5_VALIDATE:
            case CD_SCDG6_VALIDATE:
            case CD_SCDG7_VALIDATE:
            case CD_SCDG8_VALIDATE:
            case CD_SCDG9_VALIDATE:
            case CD_SCDG10_VALIDATE:
            case CD_SCDG11_VALIDATE:
            case CD_SCDG12_VALIDATE:
            case CD_SCDG13_VALIDATE:
            case CD_SCDG14_VALIDATE:
            case CD_SCDG15_VALIDATE:
            case CD_SCDG16_VALIDATE:
            case CD_SCSIGNEDATTRS_VALIDATE:
            case CD_SCSIGNATURE_VALIDATE:
            case CD_ACTIVE_AUTHENTICATION:
            case CD_SCTERMINAL_AUTHENTICATION_STATUS:
            case CD_SCCHIP_AUTHENTICATION_STATUS:
            case CD_SAC_STATUS:
            case CD_DATAPAGE_TO_CHIP_MRZ_COMPARISON:
            case CD_DATAPAGE_TO_CHIP_FACE_COMPARISON:
            case CD_SCBAC_STATUS:
                var15 = var3[0] + var3[1] * 256 + var3[2] * 65536 + var3[3] * 16777216;
                System.out.println("Data:  " + var1.toString() + " = " + var15);
                break;
            case CD_SWIPE_MSR_DATA:
                MsrData var5 = this.prFullPageReader.ConstructMsrData(var3);
                System.out.println("MSR TRACK 1: " + var5.Track1);
                System.out.println("MSR TRACK 2: " + var5.Track2);
                System.out.println("MSR TRACK 3: " + var5.Track3);
                break;
            case CD_AAMVA_DATA:
            case CD_SWIPE_AAMVA_DATA:
                AAMVAData var6 = Marshal.ConstructAAMVAData(var3);
                System.out.println("AAMVA Full Name: " + var6.Parsed.FullName);
                System.out.println("AAMVA Licence Number: " + var6.Parsed.LicenceNumber);
                break;
            case CD_BARCODE_1D_INDUSTRIAL_2_OF_5:
            case CD_BARCODE_1D_INTERLEAVED_2_OF_5:
            case CD_BARCODE_1D_IATA_2_OF_5:
            case CD_BARCODE_1D_3_OF_9:
            case CD_BARCODE_1D_128:
            case CD_BARCODE_PDF417:
            case CD_BARCODE_QRCODE:
            case CD_BARCODE_DATAMATRIX:
            case CD_BARCODE_1D_CODE_93:
            case CD_BARCODE_1D_CODABAR:
            case CD_BARCODE_1D_UPC_EAN:
                var23 = this.prFullPageReader.ConstructPluginData(var3);
                var8 = "";
                if (var23.puDataLen > 1) {
                    var8 = new String(var23.puData, 0, var23.puDataLen - 1);
                }

                System.out.println("Data:  " + var1.toString() + " - " + var8 + " - (" + var23.puDataLen + " bytes)");
                break;
            case CD_OCRTOOLKIT:
                try {
                    var23 = this.prFullPageReader.ConstructPluginData(var3);
                    var8 = "";
                    switch(var23.puDataFormat) {
                        case STRING:
                        case STRING_ASCII:
                            var8 = new String(var23.puData, 0, var23.puDataLen, "ISO-8859-1");
                            break;
                        case STRING_UNICODE:
                            var8 = new String(var23.puData, 0, var23.puDataLen, "UTF-16");
                            break;
                        default:
                            var8 = var23.puDataFormat.toString() + " - (" + var23.puDataLen + " bytes)";
                    }

                    if (var23.puFieldName.length() > 0) {
                        var8 = var23.puFieldName + ": " + var8;
                    }

                    if (var23.puFeatureName.length() > 0) {
                        var8 = var23.puFeatureName + ": " + var8;
                    }

                    System.out.println(var8);
                } catch (Exception var14) {
                    System.out.println("PluginData received. Error occured parsing data");
                }
                break;
            case CD_BACKEY_CORRECTION:
                String var21 = "";
                var8 = "";
                String var9 = new String(var3, 0, var2 - 1);
                var9 = var9.trim();
                System.out.println("Data:  " + var1.toString() + " - " + var9);
                int var10 = var9.length();
                if (var10 > 0) {
                    if (var10 > 44) {
                        var10 = 44;
                        var8 = var9.substring(var10);
                    }

                    var21 = var9.substring(0, var10);
                }
                int var12 = 0;

                int var13;
                for(var13 = 0; var13 < var21.length(); ++var13) {
                    var3[var12++] = (byte)var21.charAt(var13);
                }

                for(var13 = 0; var13 < var8.length(); ++var13) {
                    var3[var12++] = (byte)var8.charAt(var13);
                }

                return;
            case CD_READ_PROGRESS:
                float var19 = ByteBuffer.wrap(var3).order(ByteOrder.LITTLE_ENDIAN).getFloat();
                System.out.println("Data:  " + var1.toString() + " = " + var19);
                break;
            case CD_DETECT_PROGRESS:
                int var18 = var3[0] + var3[1] * 256 + var3[2] * 65536 + var3[3] * 16777216;
                var8 = "NoDocument";
                if (var18 == 1) {
                    var8 = "MovingDocument";
                } else if (var18 == 2) {
                    var8 = "FoundDocument";
                }

                if (this.prDetectState != var18) {
                    this.prDetectState = var18;
                    System.out.println("Data:  " + var1.toString() + " = " + var8);
                } else {
                    if (this.prDetectStateCounter > 6) {
                        System.out.println("Data:  " + var1.toString() + " = " + var8);
                        this.prDetectStateCounter = 0;
                    }

                    ++this.prDetectStateCounter;
                }
                break;
            case CD_DGC_SIGNATURE_VALIDATE:
                var15 = var3[0] + var3[1] * 256 + var3[2] * 65536 + var3[3] * 16777216;
                System.out.println("Data:  " + var1.toString() + " = " + var15);
                break;
            case CD_DGC_DOC_SIGNER_CERT_VALIDATE:
                var15 = var3[0] + var3[1] * 256 + var3[2] * 65536 + var3[3] * 16777216;
                System.out.println("Data:  " + var1.toString() + " = " + var15);
                break;
            default:
                System.out.println("Data:  " + var1.toString() + " = <unknown - " + var2 + " bytes>");
        }

    }

    public void OnMMMReaderError(ErrorCode var1, String var2) {
        System.out.println("Error: " + var1.toString() + " - " + var2);
    }

    public void OnFullPageReaderEvent(EventCode var1) {
        System.out.println("Event: " + var1.toString());
        switch(var1) {
            case SETTINGS_INITIALISED:
                Package var5 = Package.getPackage("com.mmm.readers.FullPage");
                System.out.println("Package represented by myPackage: " + var5.toString());
                System.out.println("Implementation vendor of myPackage: " + var5.getImplementationVendor());
                System.out.println("Implementation version of myPackage: " + var5.getImplementationVersion());
                break;
            case PLUGINS_INITIALISED:
                StringBuffer var2 = new StringBuffer("");
                boolean[] var3 = new boolean[1];
                int var4 = 0;

                do {
                    this.prFullPageReader.GetPluginName(var2, var4);
                    this.prFullPageReader.IsPluginEnabled(var2.toString(), var3);
                    ++var4;
                } while(var2.length() > 0);

                return;
            case READER_STATE_CHANGED:
                System.out.println(this.prFullPageReader.GetState().toString());
        }

    }

    public boolean OnRFIDCertificateCallback(char[] var1, int var2, CertificateType var3, CertificateObject var4) {
        boolean var5 = false;
        JFileChooser var6 = new JFileChooser(this.prCurrentCertDir);
        var6.addChoosableFileFilter(new FileNameExtensionFilter("All Certificates (cer, crl, crt, der, pem, cvcert, pkcs8)", new String[]{"cer", "crl", "crt", "der", "pem", "cvcert", "pkcs8"}));
        String var7 = new String(var1);
        String var8;
        switch(var3) {
            case CT_DOC_SIGNER_CERT:
                var8 = "Open Document Signer Certificate";
                break;
            case CT_COUNTRY_SIGNER_CERT:
                var8 = "Open Country Signer Certificate";
                break;
            case CT_CVCA_CERT:
                var8 = "Open CV Link Certificate";
                break;
            case CT_DV_CERT:
                var8 = "Open DV Certificate";
                break;
            case CT_IS_CERT:
                var8 = "Open IS Certificate";
                break;
            case CT_IS_PRIVATE_KEY:
                var8 = "Open IS Private Key";
                break;
            default:
                var8 = "Open unknown certificate type '" + var3.toString() + "'";
        }
        var8 = var8 + " - " + var7;
        var6.setDialogTitle(var8);

        return var5;
    }

    private String FormatDate(Date var1) {
        String var2 = String.format("%02d", var1.Year) + "-" + String.format("%02d", var1.Month) + "-" + String.format("%02d", var1.Day);
        return var2;
    }
}