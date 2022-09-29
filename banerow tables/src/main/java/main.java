import org.checkerframework.checker.units.qual.A;

import java.lang.reflect.GenericDeclaration;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

public class main {

    public static void main(String[] args) {
        String ID = "92202";
        String s = "";
        List<byte[]> hashes = new ArrayList<>();
        try {
        for (int j = 0; j < 1000; j++) {
            for (int i = 0; i < 6; i++) {
                int randomNum = 0 + (int) (Math.random() * 10);
                s = s + String.valueOf(randomNum);
                System.out.println(s);
            }
            s = s + ID;


                MessageDigest digest = MessageDigest.getInstance("SHA-256");
                byte[] encodedhash = digest.digest(
                        s.getBytes(StandardCharsets.UTF_8));
                hashes.add(encodedhash);


        }
        for (byte[] hash : hashes){
            String enc = new String(hash, "UTF-8");
            String dec = new String(Hellman.decodeHash(hash), "UTF-8");
            assert2(enc,dec);
             dec = new String(Rainbow.decodeHash(hash), "UTF-8");
            assert2(enc,dec);
        }


//            String enc = new String(encodedhash, "UTF-8");
//            System.out.println(enc);
//            String dec = new String(Hellman.decodeHash(encodedhash), "UTF-8");
//            if(enc.equals(dec) ){
//                System.out.println("hellman succesfull");
//            }else {
//                System.out.println("hellnam unsuffcedful");
//            }
//
//            dec = new String(Rainbow.decodeHash(encodedhash), "UTF-8");
//            if(enc.equals(dec) ){
//                System.out.println("rainbow succesfull");
//            }else {
//                System.out.println("rainbow unsuffcedful");
//            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public static void assert2(String a,String b){

    }
}
