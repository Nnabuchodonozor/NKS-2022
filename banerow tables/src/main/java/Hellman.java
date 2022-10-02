import javax.swing.plaf.synth.SynthOptionPaneUI;
import java.util.Arrays;
import java.util.Map;

public class Hellman extends BigClass {

    static int m, t;

    public Hellman() throws Exception {


    }

    public void setMT(int m, int t) {
        this.m = m;
        this.t = t;
    }

    public static String solveHash(byte[] encodedHash) throws Exception {
        //reducing cipher text

        System.out.println();
        String reducedCT = reductionFunction(encodedHash, 0);

        //
        // searching endpoints
        String potentialOpenText = "";
        for(int i = 0; i < t; i++) {
            for (String endpoint : theTable.values()) {
                if (reducedCT.equals(endpoint)) {
                    //nasli sme endpoint, prehladavame chain
                     potentialOpenText = iterateChain(encodedHash, endpoint);
                     break;
                }
            }
                    if (potentialOpenText != null) {
                        System.out.println("correct input was " + potentialOpenText + " with hash " + toHex(encodedHash));
                        return potentialOpenText;
                    } else {
                        System.out.println(" no endpoint was matchig, proceeding to hash potential");
                        reducedCT = reductionFunction(stringToSHA(reducedCT), 0);
                    }

            }

        System.out.println("unable to find plaintext for cipher text: " + toHex(encodedHash));
        return null;
    }

    private static String iterateChain(byte[] cipherText, String endPoint) throws Exception {
        String startingPoint = "";
        for (Map.Entry<String, String> entry : theTable.entrySet()) {
            if (endPoint.equals(entry.getValue())) {
                startingPoint = entry.getKey();
            }
        }

        String currentString = startingPoint;
        byte[] currentByte = null;

        for (int i = 0; i < t; i++) {
             currentByte = stringToSHA(currentString);
            if (Arrays.equals(currentByte, cipherText)) {
                return startingPoint;
            } else {
                currentString = reductionFunction(currentByte, 0);
                //currentByte = stringToSHA(currentString);
            }

        }
        return null;
    }

    public static void generateHashTables() throws Exception {

        do {
//            String startingPoint = generateID();
            String startingPoint = "00000092202";
            byte[] current = stringToSHA(startingPoint);

            for (int j = 0; j < t; j++) {
                current = stringToSHA(reductionFunction(current, 0));
            }
            String endPoint = reductionFunction(current, 0);
            theTable.put(startingPoint, endPoint);
        } while (theTable.size() < m);
    }


}
