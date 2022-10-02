import org.checkerframework.checker.units.qual.A;

import java.lang.reflect.GenericDeclaration;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class main {

    public static void main(String[] args) {


        String ID = "92202";


        HashMap<String,byte[]> hashes = new HashMap<>();
        try {
            for (int j = 0; j < 1000; j++) {
                String s = "";
                for (int i = 0; i < 6; i++) {
                    int randomNum = 0 + (int) (Math.random() * 10);
                    s = s + String.valueOf(randomNum);
                    System.out.println(s);
                }
//                s = s + ID;
                s = "000000" + ID;

                MessageDigest digest = MessageDigest.getInstance("SHA-256");
                byte[] encodedhash = digest.digest(
                        s.getBytes(StandardCharsets.UTF_8));
                hashes.put(s,encodedhash);


            }
            Hellman hellman = new Hellman();
            hellman.setMT(10,20);
            hellman.generateHashTables();
            Hellman.solveHash(hashes.entrySet().iterator().next().getValue());
//        for (byte[] hash : hashes){
//            String enc = new String(hash, "UTF-8");
//            String dec = new String(Hellman.decodeHash(hash), "UTF-8");
//            assert2(enc,dec);
//             dec = new String(Rainbow.decodeHash(hash), "UTF-8");
//            assert2(enc,dec);
//            }
        } catch (Exception e) {
            e.printStackTrace();
        }

//
//
//    public static void assert2(String a,String b){
//
//    }
    }
}