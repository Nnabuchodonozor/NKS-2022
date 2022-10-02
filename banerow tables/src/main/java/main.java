import org.apache.commons.codec.binary.Hex;
import org.checkerframework.checker.units.qual.A;

import java.lang.reflect.GenericDeclaration;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class main {

    public static void main(String[] args) {



        Instant start = Instant.now();


        String ID = "92202";
        HashMap<String,byte[]> hashes = new HashMap<>();
        try {
            for (int j = 0; j < 1000; j++) {
                String s = "";
                for (int i = 0; i < 6; i++) {
                    int randomNum = 0 + (int) (Math.random() * 10);
                    s = s + String.valueOf(randomNum);
                }
//                s = s + ID;
                s = s + ID;

                MessageDigest digest = MessageDigest.getInstance("SHA-256");
                byte[] encodedhash = digest.digest(
                        s.getBytes(StandardCharsets.UTF_8));
                hashes.put(s,encodedhash);


            }

            Hellman hellman = new Hellman();
            hellman.setMT(100,100);
            hellman.generateHashTables();

            int correct = 0;
            for (Map.Entry<String, byte[]> entry : hashes.entrySet()) {
                System.out.println(" plain: "+ entry.getKey() + " , hash: \t\t\t\t\t\t"+new String(toHex(entry.getValue())));
               if (Hellman.solveHash(entry.getValue())){
                   correct++;
               }
            }
            System.out.println("correct: "+  correct + "/1000");

        } catch (Exception e) {
            e.printStackTrace();
        }

        Instant end = Instant.now();
        Duration timeElapsed = Duration.between(start, end);
        System.out.println("Time taken: "+ timeElapsed.toMillis() +" milliseconds");

    }
    public static char[] toHex(byte[] inp) {
        return Hex.encodeHex(inp);
    }
}