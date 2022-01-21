//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)
//

package com.reader;

import com.mmm.readers.ErrorCode;
import com.mmm.readers.ErrorHandler;
import com.mmm.readers.FullPage.DataHandler;
import com.mmm.readers.FullPage.DataType;
import com.mmm.readers.FullPage.EventHandler;
import com.mmm.readers.FullPage.Reader;
import com.mmm.readers.modules.rfid.CertificateHandler;
import java.awt.Component;
import java.awt.EventQueue;
import java.awt.List;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.GroupLayout.Alignment;
import javax.swing.LayoutStyle.ComponentPlacement;

public class ScannerBlocking extends JFrame implements ErrorHandler {
    private JButton btnInitialise;
    private JButton btnReadDocument;
    private JButton btnShutdown;
    private JButton btnWaitForDocument;
    private JScrollPane jScrollPane1;
    private List lstMessages;
    private JTextArea txtCodeline;
    private Reader prFullPageReader;

    public ScannerBlocking() {
        this.initComponents();
    }

    private void initComponents() {
        this.lstMessages = new List();
        this.jScrollPane1 = new JScrollPane();
        this.txtCodeline = new JTextArea();
        this.btnInitialise = new JButton();
        this.btnWaitForDocument = new JButton();
        this.btnReadDocument = new JButton();
        this.btnShutdown = new JButton();
        this.setDefaultCloseOperation(3);
        this.txtCodeline.setColumns(20);
        this.txtCodeline.setRows(3);
        this.jScrollPane1.setViewportView(this.txtCodeline);
        this.btnInitialise.setText("Initialise");
        this.btnInitialise.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent var1) {
                ScannerBlocking.this.btnInitialiseActionPerformed(var1);
            }
        });
        this.btnWaitForDocument.setText("WaitForDocument");
        this.btnWaitForDocument.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent var1) {
                ScannerBlocking.this.btnWaitForDocumentActionPerformed(var1);
            }
        });
        this.btnReadDocument.setText("ReadDocument");
        this.btnReadDocument.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent var1) {
                ScannerBlocking.this.btnReadDocumentActionPerformed(var1);
            }
        });
        this.btnShutdown.setText("Shutdown");
        this.btnShutdown.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent var1) {
                ScannerBlocking.this.btnShutdownActionPerformed(var1);
            }
        });
        GroupLayout var1 = new GroupLayout(this.getContentPane());
        this.getContentPane().setLayout(var1);
        var1.setHorizontalGroup(var1.createParallelGroup(Alignment.LEADING).addGroup(var1.createSequentialGroup().addContainerGap().addGroup(var1.createParallelGroup(Alignment.LEADING).addGroup(var1.createSequentialGroup().addGroup(var1.createParallelGroup(Alignment.LEADING).addComponent(this.lstMessages, -1, 492, 32767).addComponent(this.jScrollPane1, -1, 492, 32767)).addContainerGap()).addGroup(var1.createSequentialGroup().addComponent(this.btnInitialise).addPreferredGap(ComponentPlacement.RELATED).addComponent(this.btnWaitForDocument).addPreferredGap(ComponentPlacement.RELATED).addComponent(this.btnReadDocument).addPreferredGap(ComponentPlacement.RELATED).addComponent(this.btnShutdown).addGap(8, 8, 8)))));
        var1.linkSize(0, new Component[]{this.btnInitialise, this.btnReadDocument, this.btnShutdown, this.btnWaitForDocument});
        var1.setVerticalGroup(var1.createParallelGroup(Alignment.LEADING).addGroup(var1.createSequentialGroup().addContainerGap().addComponent(this.lstMessages, -1, 227, 32767).addPreferredGap(ComponentPlacement.RELATED).addComponent(this.jScrollPane1, -2, 68, -2).addGap(18, 18, 18).addGroup(var1.createParallelGroup(Alignment.BASELINE).addComponent(this.btnInitialise).addComponent(this.btnWaitForDocument).addComponent(this.btnReadDocument).addComponent(this.btnShutdown)).addContainerGap()));
        this.pack();
    }

    private void btnInitialiseActionPerformed(ActionEvent var1) {
        try {
            this.prFullPageReader = new Reader();
            this.AddMessageText("Initialising...");
            ErrorCode var2 = this.prFullPageReader.Initialise((DataHandler)null, (EventHandler)null, this, (CertificateHandler)null, true, false, 0);
            if (var2 != ErrorCode.NO_ERROR_OCCURRED) {
                this.AddMessageText("Initialise error: " + var2.toString());
            } else {
                this.AddMessageText("Initialise successful");
            }

            StringBuffer var3 = new StringBuffer("");
            boolean[] var4 = new boolean[1];
            this.AddMessageText("Turn off plugins: ");
            int var5 = 0;

            do {
                this.prFullPageReader.GetPluginName(var3, var5);
                if (var3.length() > 0) {
                    this.AddMessageText(" - " + var3.toString());
                    this.prFullPageReader.IsPluginEnabled(var3.toString(), var4);
                    if (var4[0]) {
                        this.prFullPageReader.EnablePlugin(var3.toString(), false);
                    }
                }

                ++var5;
            } while(var3.length() > 0);

            this.AddMessageText("Plugins turned off");
        } catch (Throwable var6) {
            JOptionPane.showMessageDialog(this, "Unable to initialise " + var6.toString(), "Error", 0);
        }

    }

    private void btnWaitForDocumentActionPerformed(ActionEvent var1) {
        ErrorCode var2 = this.prFullPageReader.WaitForDocumentOnWindow(5000);
        if (var2 == ErrorCode.NO_ERROR_OCCURRED) {
            JOptionPane.showMessageDialog(this, "Found Document", "Information", 1);
        } else if (var2 == ErrorCode.ERROR_TIMED_OUT) {
            JOptionPane.showMessageDialog(this, "Timed out waiting for document", "Information", 1);
        } else {
            JOptionPane.showMessageDialog(this, "Errored waiting for document - " + var2.toString(), "Information", 1);
        }

    }

    private void btnReadDocumentActionPerformed(ActionEvent var1) {
        if (this.prFullPageReader.IsDocumentOnWindow()) {
            ErrorCode var2 = this.prFullPageReader.ReadDocument();
            if (var2 == ErrorCode.NO_ERROR_OCCURRED) {
                int var3 = 300000;
                byte[] var4 = new byte[200];
                int[] var5 = new int[]{200};
                if (this.prFullPageReader.GetData(DataType.CD_CODELINE, var4, var5) == ErrorCode.NO_ERROR_OCCURRED) {
                    String var6 = new String(var4, 0, var5[0] - 1);
                    if (var6.startsWith("P")) {
                        var6 = var6.substring(0, 43) + "\r\n" + var6.substring(44, var6.length());
                    }

                    this.txtCodeline.setText(var6);
                }

                byte[] var8 = new byte[var3];
                int[] var7 = new int[]{var3};
                if (this.prFullPageReader.GetData(DataType.CD_IMAGEIR, var8, var7) == ErrorCode.NO_ERROR_OCCURRED) {
                }
            }
        }

    }

    private void btnShutdownActionPerformed(ActionEvent var1) {
        if (this.prFullPageReader != null) {
            this.AddMessageText("Shutting down scanner...");
            ErrorCode var2 = this.prFullPageReader.Shutdown();
            if (var2 == ErrorCode.NO_ERROR_OCCURRED) {
                this.AddMessageText("Shutdown success");
            } else {
                this.AddMessageText("Error shutting down scanner - " + var2.toString());
            }
        }

    }

    public static void main(String[] var0) {
        EventQueue.invokeLater(new Runnable() {
            public void run() {
                (new ScannerBlocking()).setVisible(true);
            }
        });
    }

    public void OnMMMReaderError(ErrorCode var1, String var2) {
        System.out.println(var2);
        this.AddMessageText(var2);
    }

    private void AddMessageText(String var1) {
        this.lstMessages.add(var1);
        this.lstMessages.makeVisible(this.lstMessages.getItemCount() - 1);
    }
}