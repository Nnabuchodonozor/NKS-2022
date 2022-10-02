import java.util.Arrays;
import java.util.Map;

public class Rainbow extends BigClass{

     static int m,t;

    public Rainbow() {
    }

    public void setMT(int m, int t) {
        this.m = m;
        this.t = t;
    }

    public static boolean solveHash(byte[] encodedHash, int parameter) throws Exception {
        //reducing cipher text
        parameter = parameter % 64;
        String reducedCT = reductionFunction(encodedHash, parameter);

        //
        // searching endpoints
        String potentialOpenText = null;
        for(int i = 0; i < t; i++) {
            for (String endpoint : theTable.values()) {
                if (reducedCT.equals(endpoint)) {
                    //nasli sme endpoint, prehladavame chain
                    potentialOpenText = iterateChain(encodedHash, endpoint, parameter);
                    break;
                }
            }
            if (potentialOpenText != null) {
                System.out.println("correct input was " + potentialOpenText + " with hash \t\t\t\t\t" + new String( toHex(encodedHash)));
                return true;
            } else {
//                        System.out.println(" no endpoint was matchig for: " + reducedCT +" , proceeding to hash ");
                reducedCT = reductionFunction(stringToSHA(reducedCT), parameter);
            }

        }

        System.out.println("unable to find plaintext for cipher text: \t\t" + new String( toHex(encodedHash)));
        return false;
    }

    private static String iterateChain(byte[] cipherText, String endPoint, int parameter) throws Exception {
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
                return currentString;
            } else {
                currentString = reductionFunction(currentByte, parameter);
                //currentByte = stringToSHA(currentString);
            }

        }
        return null;
    }

    public static void generateHashTables() throws Exception {

        do {
            String startingPoint = generateID();
//            String startingPoint = "00000092202";
            byte[] current = stringToSHA(startingPoint);

            for (int j = 0; j < t; j++) {
                current = stringToSHA(reductionFunction(current, t % 64));
            }
            String endPoint = reductionFunction(current, t % 64);
            theTable.put(startingPoint, endPoint);
        } while (theTable.size() < m);
    }

}
