package com.reader;

import java.awt.EventQueue;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JTextField;
import javax.swing.GroupLayout.Alignment;
import javax.swing.LayoutStyle.ComponentPlacement;

public class BACKeyCorrectionDialog extends JDialog {
    private JButton btnOK;
    private JButton btnUndo;
    private JTextField txtCodeline1;
    private JTextField txtCodeline2;
    private String prOriginalCodeline1;
    private String prOriginalCodeline2;

    public BACKeyCorrectionDialog(Frame var1, boolean var2) {
        super(var1, var2);
        this.initComponents();
    }

    private void initComponents() {
        this.txtCodeline2 = new JTextField();
        this.txtCodeline1 = new JTextField();
        this.btnOK = new JButton();
        this.btnUndo = new JButton();
        this.setDefaultCloseOperation(2);
        this.setTitle("Edit Codeline Data for BAC");
        this.setResizable(false);
        this.addWindowListener(new WindowAdapter() {
            public void windowClosed(WindowEvent var1) {
                com.reader.BACKeyCorrectionDialog.this.formWindowClosed(var1);
            }
        });
        this.txtCodeline1.setText("P<GBRSAMPLE<<NATACHA<<<<<<<<<<<<<<<<<<<<<<<<");
        this.btnOK.setText("OK");
        this.btnOK.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent var1) {
                com.reader.BACKeyCorrectionDialog.this.btnOKActionPerformed(var1);
            }
        });
        this.btnUndo.setText("Undo");
        this.btnUndo.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent var1) {
                com.reader.BACKeyCorrectionDialog.this.btnUndoActionPerformed(var1);
            }
        });
        GroupLayout var1 = new GroupLayout(this.getContentPane());
        this.getContentPane().setLayout(var1);
        var1.setHorizontalGroup(var1.createParallelGroup(Alignment.LEADING).addGroup(var1.createSequentialGroup().addContainerGap().addGroup(var1.createParallelGroup(Alignment.TRAILING, false).addComponent(this.txtCodeline1, Alignment.LEADING).addComponent(this.txtCodeline2, Alignment.LEADING, -1, 343, 32767)).addContainerGap()).addGroup(Alignment.TRAILING, var1.createSequentialGroup().addContainerGap(107, 32767).addComponent(this.btnOK, -2, 75, -2).addPreferredGap(ComponentPlacement.RELATED).addComponent(this.btnUndo, -2, 75, -2).addGap(100, 100, 100)));
        var1.setVerticalGroup(var1.createParallelGroup(Alignment.LEADING).addGroup(var1.createSequentialGroup().addContainerGap().addComponent(this.txtCodeline1, -2, -1, -2).addPreferredGap(ComponentPlacement.RELATED).addComponent(this.txtCodeline2, -2, -1, -2).addPreferredGap(ComponentPlacement.RELATED, 22, 32767).addGroup(var1.createParallelGroup(Alignment.BASELINE).addComponent(this.btnOK).addComponent(this.btnUndo)).addContainerGap()));
        this.pack();
    }

    private void btnUndoActionPerformed(ActionEvent var1) {
        this.ResetCodelineText();
    }

    private void btnOKActionPerformed(ActionEvent var1) {
        this.dispose();
    }

    private void formWindowClosed(WindowEvent var1) {
        this.ResetCodelineText();
    }

    public static void main(String[] var0) {
        EventQueue.invokeLater(new Runnable() {
            public void run() {
                com.reader.BACKeyCorrectionDialog var1 = new com.reader.BACKeyCorrectionDialog(new JFrame(), true);
                var1.addWindowListener(new WindowAdapter() {
                    public void windowClosing(WindowEvent var1) {
                        System.exit(0);
                    }
                });
                var1.setVisible(true);
            }
        });
    }

    public void SetCodelineText(String var1, String var2) {
        this.prOriginalCodeline1 = var1;
        this.prOriginalCodeline2 = var2;
        this.ResetCodelineText();
    }

    public String GetCodeline1Text() {
        return this.txtCodeline1.getText();
    }

    public String GetCodeline2Text() {
        return this.txtCodeline2.getText();
    }

    private void ResetCodelineText() {
        this.txtCodeline1.setText(this.prOriginalCodeline1);
        this.txtCodeline2.setText(this.prOriginalCodeline2);
    }
}
