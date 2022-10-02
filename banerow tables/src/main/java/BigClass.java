import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.HashMap;

import org.apache.commons.codec.binary.Hex;

public class BigClass {
    static HashMap<String, String> theTable = new HashMap<>();

    static int m, t;

    public BigClass() {
    }

    protected static char[] toHex(byte[] inp) {
        return Hex.encodeHex(inp);
    }

    protected static byte[] toByte(char[] s) throws Exception {
        return Hex.decodeHex(s);
    }


    protected static byte[] stringToSHA(String inpout) throws Exception {
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        return digest.digest(
                inpout.getBytes(StandardCharsets.UTF_8));
    }

    protected static String generateID() {
        String s = "";
        for (int i = 0; i < 6; i++) {
            int randomNum = 0 + (int) (Math.random() * 10);
            s = s + String.valueOf(randomNum);
        }
        return s + "92202";
    }

    protected static String reductionFunction(byte[] a, int parameter) throws Exception {

        char[] hexHash = toHex(a);
        String s = new String(hexHash);
        String PIN = "";

        if (parameter != 0) {
            int lengthParam = 6 * parameter;
            String newS = s;
            while (newS.length() < lengthParam) {
                newS += s;
            }

            int i = parameter;
            while (PIN.length() != 6) {
                PIN += newS.charAt(i);
                i = i + parameter;
            }
        }else {
            PIN = s.substring(0,6);
        }

//        System.out.println(hexHash);
//        byte[] c = toByte(hexHash);
        BigInteger bigInt = new BigInteger(PIN, 16);
        String tmp = bigInt.toString();
        while (tmp.length() < 6){
            tmp = "0" + tmp;
        }
        PIN = tmp.substring(0,6);

        return PIN + "92202";
    }

    ;
}


