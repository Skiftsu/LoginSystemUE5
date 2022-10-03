package com.server.Utils;

import javax.crypto.*;
import javax.crypto.interfaces.DHPublicKey;
import javax.crypto.spec.DHParameterSpec;
import javax.crypto.spec.DHPublicKeySpec;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.math.BigInteger;
import java.security.*;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;
import java.util.Base64;

public class Cryptography
{
    public Object[] Encrypt(String InMessage)
    {
        try
        {
            byte[] IV = new byte[16];
            new SecureRandom().nextBytes(IV);
            IvParameterSpec IvParameter = new IvParameterSpec(IV);

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            cipher.init(Cipher.ENCRYPT_MODE, SharedSecretKey, IvParameter);
            byte[] EncryptedMsg = cipher.doFinal(InMessage.getBytes());
            return new Object[]{new String(Base64.getEncoder().encode(EncryptedMsg)), new String(Base64.getEncoder().encode(IV))};
        }
        catch (InvalidKeyException | NoSuchPaddingException | NoSuchAlgorithmException | IllegalBlockSizeException | BadPaddingException | InvalidAlgorithmParameterException e)
        {e.printStackTrace();}
        return null;
    }

    public byte[] Decrypt(String InMessage, String IV)
    {
        try
        {
            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            cipher.init(Cipher.DECRYPT_MODE, SharedSecretKey, new IvParameterSpec(Base64.getDecoder().decode(IV)));
            return cipher.doFinal(Base64.getDecoder().decode(InMessage));
        }
        catch (IllegalBlockSizeException | BadPaddingException | NoSuchPaddingException | NoSuchAlgorithmException | InvalidKeyException | InvalidAlgorithmParameterException e)
        {e.printStackTrace();}
        return null;
    }

    /* ========================== Diffie Hellman ========================== */
    public SecretKeySpec SharedSecretKey;
    private KeyAgreement DhKeyExchange;
    private static final byte[] dh_p = {
            (byte)0xEB, (byte)0x61, (byte)0x99, (byte)0x83, (byte)0x8E, (byte)0x4E, (byte)0x30, (byte)0xFD, (byte)0x0F, (byte)0xC0,
            (byte)0x4E, (byte)0x1B, (byte)0x69, (byte)0xDB, (byte)0x3D, (byte)0x85, (byte)0x59, (byte)0x4D, (byte)0xB8, (byte)0x55,
            (byte)0x08, (byte)0xAF, (byte)0x9D, (byte)0x5D, (byte)0x83, (byte)0xB7, (byte)0x60, (byte)0x20, (byte)0xA0, (byte)0xCE,
            (byte)0x22, (byte)0x23, (byte)0x8D, (byte)0x00, (byte)0x72, (byte)0x06, (byte)0xA9, (byte)0xC0, (byte)0x55, (byte)0x01,
            (byte)0x31, (byte)0xB9, (byte)0xFB, (byte)0x73, (byte)0xD2, (byte)0x1F, (byte)0xF0, (byte)0x0C, (byte)0xFF, (byte)0xA2,
            (byte)0xCE, (byte)0x8F, (byte)0xB1, (byte)0xCA, (byte)0x64, (byte)0xA3, (byte)0xBF, (byte)0x46, (byte)0x61, (byte)0x45,
            (byte)0xB6, (byte)0xD1, (byte)0xD7, (byte)0x2B};

    private static final BigInteger P = new BigInteger(1, dh_p);
    private static final BigInteger G = BigInteger.valueOf(2);
    public String GenerateKeyPair_DH()
    {
        try
        {
            KeyPairGenerator keypairGenerator = KeyPairGenerator.getInstance("DH");
            keypairGenerator.initialize(new DHParameterSpec(P, G));
            KeyPair keyPair = keypairGenerator.generateKeyPair();

            DHPublicKey dhPub = (DHPublicKey)keyPair.getPublic();

            DhKeyExchange = KeyAgreement.getInstance("DH");
            DhKeyExchange.init(keyPair.getPrivate());
            return String.valueOf(dhPub.getY());
        }
        catch (InvalidAlgorithmParameterException | NoSuchAlgorithmException | InvalidKeyException e)
        {e.printStackTrace();}
        return "";
    }

    public void ComputeSharedSecretKey_DH(String PublicKey)
    {
        try
        {
            DHPublicKeySpec dhPubKeySpecs = new DHPublicKeySpec(new BigInteger(PublicKey), P, G);
            KeyFactory kf = KeyFactory.getInstance("DH");
            DHPublicKey PubKey = (DHPublicKey) kf.generatePublic(dhPubKeySpecs);

            DhKeyExchange.doPhase(PubKey, true);
            SharedSecretKey = new SecretKeySpec(Arrays.copyOfRange(DhKeyExchange.generateSecret(), 0, 32), "AES");
            System.out.println("Successful secret key generation");
        }
        catch (NoSuchAlgorithmException | InvalidKeySpecException | InvalidKeyException e)
        {e.printStackTrace();}
    }
}