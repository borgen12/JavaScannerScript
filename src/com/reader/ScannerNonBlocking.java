package com.reader;

import com.mmm.readers.AAMVAData;
import com.mmm.readers.CodelineData;
import com.mmm.readers.Date;
import com.mmm.readers.DigitalGreenCertificateData;
import com.mmm.readers.ErrorCode;
import com.mmm.readers.ErrorHandler;
import com.mmm.readers.FullPage.DataHandler;
import com.mmm.readers.FullPage.DataType;
import com.mmm.readers.FullPage.EventCode;
import com.mmm.readers.FullPage.EventHandler;
import com.mmm.readers.FullPage.PluginData;
import com.mmm.readers.FullPage.Reader;
import com.mmm.readers.interop.Marshal;
import com.mmm.readers.modules.Swipe.MsrData;
import com.mmm.readers.modules.rfid.CertificateHandler;
import com.mmm.readers.modules.rfid.CertificateObject;
import com.mmm.readers.modules.rfid.CertificateType;
import com.socket.SocketClient;

import java.awt.Component;
import java.awt.EventQueue;
import java.awt.List;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.text.SimpleDateFormat;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.GroupLayout.Alignment;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.filechooser.FileNameExtensionFilter;

public class ScannerNonBlocking extends JFrame implements DataHandler, ErrorHandler, EventHandler, CertificateHandler {
    private JButton btnInitialise;
    private JButton btnShutdown;
    private List puMsgList;
    private Reader prFullPageReader;
    private File prCurrentCertDir;
    private boolean prInitialised;
    private int prDetectState;
    private int prDetectStateCounter;
    private SocketClient socket;

    public ScannerNonBlocking()throws IOException {
       // this.startSocket();
        this.initComponents();
        this.prInitialised = false;
        this.prDetectState = 0;
        this.prDetectStateCounter = 0;
    }

    public void startSocket() throws IOException {
        socket = new SocketClient();
        socket.startConnection();
    }

    public void sendSocketMessage(String data) {
        try {
            this.socket.sendMessage(data);
        }
        catch(Throwable e) {
            //this.shutdownReader();
        }
    }

    private void initComponents() {
        this.btnInitialise = new JButton();
        this.btnShutdown = new JButton();
        this.puMsgList = new List();
        this.setDefaultCloseOperation(3);
        this.setTitle("Full Page Reader - Non-Blocking Mode - Java");
        this.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent var1) {
                ScannerNonBlocking.this.formWindowClosing(var1);
            }
        });
        this.btnInitialise.setText("Initialise");
        this.btnInitialise.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent var1) {
                ScannerNonBlocking.this.btnInitialiseActionPerformed(var1);
            }
        });
        this.btnShutdown.setText("Shutdown");
        this.btnShutdown.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent var1) {
                ScannerNonBlocking.this.btnShutdownActionPerformed(var1);
            }
        });
        GroupLayout var1 = new GroupLayout(this.getContentPane());
        this.getContentPane().setLayout(var1);
        var1.setHorizontalGroup(var1.createParallelGroup(Alignment.LEADING).addGroup(var1.createSequentialGroup().addContainerGap().addGroup(var1.createParallelGroup(Alignment.LEADING).addComponent(this.btnInitialise).addComponent(this.btnShutdown)).addPreferredGap(ComponentPlacement.RELATED).addComponent(this.puMsgList, -1, 463, 32767).addContainerGap()));
        var1.linkSize(0, new Component[]{this.btnInitialise, this.btnShutdown});
        var1.setVerticalGroup(var1.createParallelGroup(Alignment.LEADING).addGroup(var1.createSequentialGroup().addContainerGap().addGroup(var1.createParallelGroup(Alignment.LEADING).addComponent(this.puMsgList, -1, 370, 32767).addGroup(var1.createSequentialGroup().addComponent(this.btnInitialise).addPreferredGap(ComponentPlacement.RELATED).addComponent(this.btnShutdown))).addContainerGap()));
        this.pack();
    }

    private void btnInitialiseActionPerformed(ActionEvent var1) {
        try {
            this.AddMsgToMsgList("Loading Highlevel dll...");
            this.prFullPageReader = new Reader();
            this.AddMsgToMsgList("Highlevel dll loaded");
            this.prFullPageReader.EnableLogging(true, 5, -1, "NonBlockingJava.log");
            this.AddMsgToMsgList("Initialising...");
            ErrorCode var2 = this.prFullPageReader.Initialise(this, this, this, this, true, false, 0);
            if (var2 != ErrorCode.NO_ERROR_OCCURRED) {
                if (var2 == ErrorCode.ERROR_MISMATCH_IN_AN_ENUM) {
                    JOptionPane.showMessageDialog(this, "ERROR: Mismatch in an Enum");
                } else if (var2 == ErrorCode.ERROR_MISMATCH_SETTING_SIZE) {
                    JOptionPane.showMessageDialog(this, "ERROR: Mismatch in the Settings");
                } else {
                    this.AddMsgToMsgList("Initialise error: " + var2.toString());
                }
            } else {
                this.AddMsgToMsgList("Initialise successful");
                this.prInitialised = true;
            }
        } catch (Throwable var3) {
            JOptionPane.showMessageDialog(this, "Unable to initialise " + var3.toString());
            var3.printStackTrace();
        }

    }

    private void btnShutdownActionPerformed(ActionEvent var1) {
        this.shutdownReader();
    }

    private void formWindowClosing(WindowEvent var1) {
        if (this.prInitialised) {
            this.shutdownReader();
        }

    }

    public static void main(String[] var0) throws IOException {
        EventQueue.invokeLater(new Runnable() {
            public void run() {
                try {
                    (new ScannerNonBlocking()).setVisible(true);
                }
                catch(Throwable e) {
                }
            }
        });
    }

    private void shutdownReader() {
        ErrorCode var1 = this.prFullPageReader.Shutdown();
        if (var1 == ErrorCode.NO_ERROR_OCCURRED) {
            this.AddMsgToMsgList("Shutdown successful");
        }

    }

    public void OnFullPageReaderData(DataType var1, int var2, byte[] var3) {
        System.out.println("var" + var2);
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
                this.AddMsgToMsgList("Data: " + var1.toString() + " = " + var17);
                break;
            case CD_CODELINE_DATA:
            case CD_SCDG1_CODELINE_DATA:
                System.out.println(var3.length);
                System.out.println(var1);
                CodelineData var16 = this.prFullPageReader.ConstructCodelineData(var3);
                this.sendSocketMessage(var16.Forename);
                //this.AddCodelineDataToMsgList(var16);
                break;
            case CD_IMAGEIR:
            case CD_IMAGEVIS:
            case CD_IMAGEUV:
            case CD_IMAGEPHOTO:
            case CD_IMAGEBARCODE:
            case CD_SCDG2_PHOTO:
                this.AddMsgToMsgList("Data: " + var1.toString() + " = <image - " + var2 + " bytes>");
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
                this.AddMsgToMsgList("Data:  " + var1.toString() + " = <binary data - " + var2 + " bytes>");
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
                this.AddMsgToMsgList("Data:  " + var1.toString() + " = " + var15);
                break;
            case CD_SWIPE_MSR_DATA:
                MsrData var5 = this.prFullPageReader.ConstructMsrData(var3);
                this.AddMsgToMsgList("MSR TRACK 1: " + var5.Track1);
                this.AddMsgToMsgList("MSR TRACK 2: " + var5.Track2);
                this.AddMsgToMsgList("MSR TRACK 3: " + var5.Track3);
                break;
            case CD_AAMVA_DATA:
            case CD_SWIPE_AAMVA_DATA:
                AAMVAData var6 = Marshal.ConstructAAMVAData(var3);
                this.AddMsgToMsgList("AAMVA Full Name: " + var6.Parsed.FullName);
                this.AddMsgToMsgList("AAMVA Licence Number: " + var6.Parsed.LicenceNumber);
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

                this.AddMsgToMsgList("Data:  " + var1.toString() + " - " + var8 + " - (" + var23.puDataLen + " bytes)");
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

                    this.AddMsgToMsgList(var8);
                } catch (Exception var14) {
                    this.AddMsgToMsgList("PluginData received. Error occured parsing data");
                }
                break;
            case CD_BACKEY_CORRECTION:
                String var21 = "";
                var8 = "";
                String var9 = new String(var3, 0, var2 - 1);
                var9 = var9.trim();
                this.AddMsgToMsgList("Data:  " + var1.toString() + " - " + var9);
                int var10 = var9.length();
                if (var10 > 0) {
                    if (var10 > 44) {
                        var10 = 44;
                        var8 = var9.substring(var10);
                    }

                    var21 = var9.substring(0, var10);
                }

                BACKeyCorrectionDialog var11 = new BACKeyCorrectionDialog(this, true);
                var11.SetCodelineText(var21, var8);
                var11.setVisible(true);
                var21 = var11.GetCodeline1Text();
                var8 = var11.GetCodeline2Text();
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
                this.AddMsgToMsgList("Data:  " + var1.toString() + " = " + var19);
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
                    this.AddMsgToMsgList("Data:  " + var1.toString() + " = " + var8);
                } else {
                    if (this.prDetectStateCounter > 6) {
                        this.AddMsgToMsgList("Data:  " + var1.toString() + " = " + var8);
                        this.prDetectStateCounter = 0;
                    }

                    ++this.prDetectStateCounter;
                }
                break;
            case CD_DIGITAL_GREEN_CERTIFICATE:
                this.AddMsgToMsgList("DigitalGreenCertificateData");
                DigitalGreenCertificateData var7 = this.prFullPageReader.ConstructDigitalGreenCertificateData(var3);
                this.AddMsgToMsgList("  Version: " + var7.Version);
                this.AddMsgToMsgList("  Surnames: " + var7.Surnames);
                this.AddMsgToMsgList("  StandardizedSurnames: " + var7.StandardizedSurnames);
                this.AddMsgToMsgList("  Fornames: " + var7.Fornames);
                this.AddMsgToMsgList("  StandardizedFornames: " + var7.StandardizedFornames);
                this.AddMsgToMsgList("  DateOfBirth: " + var7.DateOfBirth);
                this.AddMsgToMsgList("  HealthCertificateClaim");
                this.AddMsgToMsgList("    Algorithm: " + var7.HealthCertificateClaim.Algorithm);
                this.AddMsgToMsgList("    KeyIdentifierLen: " + var7.HealthCertificateClaim.KeyIdentifierLen);
                this.AddMsgToMsgList("    Version: " + var7.HealthCertificateClaim.Version);
                this.AddMsgToMsgList("    KeyIdentifier: " + new String(var7.HealthCertificateClaim.KeyIdentifier));
                this.AddMsgToMsgList("    Issuer: " + var7.HealthCertificateClaim.Issuer);
                this.AddMsgToMsgList("    IssueTime: " + var7.HealthCertificateClaim.IssueTime);
                this.AddMsgToMsgList("    ExpiryTime: " + var7.HealthCertificateClaim.ExpiryTime);
                this.AddMsgToMsgList("    HealthCertificate: " + var7.HealthCertificateClaim.HealthCertificate);
                this.AddMsgToMsgList("    DateRangeValid: " + var7.HealthCertificateClaim.DateRangeValid);
                this.AddMsgToMsgList("  VaccinationGroup");
                this.AddMsgToMsgList("    DoseNumber: " + var7.VaccinationGroup.DoseNumber);
                this.AddMsgToMsgList("    TotalDoses: " + var7.VaccinationGroup.TotalDoses);
                this.AddMsgToMsgList("    TargetedDiseaseOrAgent: " + var7.VaccinationGroup.TargetedDiseaseOrAgent);
                this.AddMsgToMsgList("    VaccineOrProphylaxis: " + var7.VaccinationGroup.VaccineOrProphylaxis);
                this.AddMsgToMsgList("    VaccineProduct: " + var7.VaccinationGroup.VaccineProduct);
                this.AddMsgToMsgList("    VaccineManufacturerOrHolder: " + var7.VaccinationGroup.VaccineManufacturerOrHolder);
                this.AddMsgToMsgList("    DateOfVaccination: " + var7.VaccinationGroup.DateOfVaccination);
                this.AddMsgToMsgList("    CountryAdministered: " + var7.VaccinationGroup.CountryAdministered);
                this.AddMsgToMsgList("    CertificateIssuer: " + var7.VaccinationGroup.CertificateIssuer);
                this.AddMsgToMsgList("    CertificateIdentifier: " + var7.VaccinationGroup.CertificateIdentifier);
                this.AddMsgToMsgList("  TestGroup");
                this.AddMsgToMsgList("    TargetedDiseaseOrAgent: " + var7.TestGroup.TargetedDiseaseOrAgent);
                this.AddMsgToMsgList("    TestType: " + var7.TestGroup.TestType);
                this.AddMsgToMsgList("    TestName: " + var7.TestGroup.TestName);
                this.AddMsgToMsgList("    TestDeviceIdentifier: " + var7.TestGroup.TestDeviceIdentifier);
                this.AddMsgToMsgList("    SampleCollectionTime: " + var7.TestGroup.SampleCollectionTime);
                this.AddMsgToMsgList("    TestResult: " + var7.TestGroup.TestResult);
                this.AddMsgToMsgList("    TestingFacility: " + var7.TestGroup.TestingFacility);
                this.AddMsgToMsgList("    CountryAdministered: " + var7.TestGroup.CountryAdministered);
                this.AddMsgToMsgList("    CertificateIssuer: " + var7.TestGroup.CertificateIssuer);
                this.AddMsgToMsgList("    CertificateIdentifier: " + var7.TestGroup.CertificateIdentifier);
                this.AddMsgToMsgList("  RecoveryGroup");
                this.AddMsgToMsgList("    RecoveredFromDiseaseOrAgent: " + var7.RecoveryGroup.RecoveredFromDiseaseOrAgent);
                this.AddMsgToMsgList("    DateOfFirstPositiveTest: " + var7.RecoveryGroup.DateOfFirstPositiveTest);
                this.AddMsgToMsgList("    CountryPositiveTestAdministered: " + var7.RecoveryGroup.CountryPositiveTestAdministered);
                this.AddMsgToMsgList("    CertificateIssuer: " + var7.RecoveryGroup.CertificateIssuer);
                this.AddMsgToMsgList("    DateCertificateValidFrom: " + var7.RecoveryGroup.DateCertificateValidFrom);
                this.AddMsgToMsgList("    DateCertificateValidUntil: " + var7.RecoveryGroup.DateCertificateValidUntil);
                this.AddMsgToMsgList("    CertificateIdentifier: " + var7.RecoveryGroup.CertificateIdentifier);
                break;
            case CD_DGC_SIGNATURE_VALIDATE:
                var15 = var3[0] + var3[1] * 256 + var3[2] * 65536 + var3[3] * 16777216;
                this.AddMsgToMsgList("Data:  " + var1.toString() + " = " + var15);
                break;
            case CD_DGC_DOC_SIGNER_CERT_VALIDATE:
                var15 = var3[0] + var3[1] * 256 + var3[2] * 65536 + var3[3] * 16777216;
                this.AddMsgToMsgList("Data:  " + var1.toString() + " = " + var15);
                break;
            default:
                this.AddMsgToMsgList("Data:  " + var1.toString() + " = <unknown - " + var2 + " bytes>");
        }

    }

    public void OnMMMReaderError(ErrorCode var1, String var2) {
        this.AddMsgToMsgList("Error: " + var1.toString() + " - " + var2);
    }

    public void OnFullPageReaderEvent(EventCode var1) {
        this.AddMsgToMsgList("Event: " + var1.toString());
        switch(var1) {
            case SETTINGS_INITIALISED:
                Package var5 = Package.getPackage("com.mmm.readers.FullPage");
                this.AddMsgToMsgList("Package represented by myPackage: " + var5.toString());
                this.AddMsgToMsgList("Implementation vendor of myPackage: " + var5.getImplementationVendor());
                this.AddMsgToMsgList("Implementation version of myPackage: " + var5.getImplementationVersion());
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
                this.AddMsgToMsgList(this.prFullPageReader.GetState().toString());
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
        if (var6.showOpenDialog(this) == 0) {
            File var9 = var6.getSelectedFile();
            this.prCurrentCertDir = var6.getCurrentDirectory();
            this.AddMsgToMsgList("Required Buffer Size = " + var4.GetRequiredBufferSize());
            this.AddMsgToMsgList("Cert File Size = " + var9.length());
            if ((long)var4.GetRequiredBufferSize() < var9.length()) {
                var4.SetRequiredBufferSize((int)var9.length());
                var5 = false;
            } else {
                try {
                    FileInputStream var10 = new FileInputStream(var9);
                    int var11 = 0;
                    int var12 = 0;

                    byte[] var13;
                    for(var13 = new byte[(int)var9.length()]; var11 < var13.length && (var12 = var10.read(var13, var11, var13.length - var11)) >= 0; var11 += var12) {
                    }

                    if ((long)var12 != var9.length()) {
                        throw new IOException("Error loading certificate data. Bytes Read: " + var12 + " File Length: " + var9.length());
                    }

                    var4.SetCertBuffer(var13);
                    var5 = true;
                } catch (IOException var14) {
                    var5 = false;
                    JOptionPane.showMessageDialog(this, var14.toString(), "Error", 0);
                }
            }
        }

        return var5;
    }

    private void AddMsgToMsgList(String var1) {
        this.puMsgList.add(var1);
        this.puMsgList.makeVisible(this.puMsgList.getItemCount() - 1);
    }

    private void AddCodelineDataToMsgList(CodelineData var1) {
        new SimpleDateFormat("dd/MM/yyyy");
        this.AddMsgToMsgList("CodelineData -");
        this.AddMsgToMsgList("DocType: " + var1.DocType);
        this.AddMsgToMsgList("DocID: " + var1.DocId);
        this.AddMsgToMsgList("DocNumber: " + var1.DocNumber);
        this.AddMsgToMsgList("Surname: " + var1.Surname);
        this.AddMsgToMsgList("Forenames: " + var1.Forenames);
        this.AddMsgToMsgList("Forename: " + var1.Forename);
        this.AddMsgToMsgList("SecondName: " + var1.SecondName);
        this.AddMsgToMsgList("Sex: " + var1.Sex);
        this.AddMsgToMsgList("Nationality: " + var1);
        this.AddMsgToMsgList("DateOfBirth: " + this.FormatDate(var1.DateOfBirth) + " (yy-mm-dd)");
        this.AddMsgToMsgList("IssuingState: " + var1.IssuingState);
        this.AddMsgToMsgList("ExpireData: " + this.FormatDate(var1.ExpiryDate) + " (yy-mm-dd)");
        System.out.println(var1);
        if (var1.CheckDigitDataList != null) {
            this.AddMsgToMsgList("CheckDigitCount: " + var1.CheckDigitDataList.length);
        }

        this.AddMsgToMsgList("CodelineValidationResult: " + var1.CodelineValidationResult);
    }

    private String FormatDate(Date var1) {
        String var2 = String.format("%02d", var1.Year) + "-" + String.format("%02d", var1.Month) + "-" + String.format("%02d", var1.Day);
        return var2;
    }
}
