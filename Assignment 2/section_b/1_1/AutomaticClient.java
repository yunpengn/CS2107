import java.io.PrintWriter;
import java.net.Socket;
import java.security.MessageDigest;
import java.util.Scanner;

import com.sun.org.apache.xerces.internal.impl.dv.util.HexBin;

public class AutomaticClient {
    private static final String SERVER_URL = "cs2107.spro.ink";
    private static final int PORT = 9000;
    private static MessageDigest hashing;

    public static void main(String[] args) throws Exception {
        // Use Java API to calculate MD5 value.
        hashing = MessageDigest.getInstance("MD5");

        // Create a client and connect to the server.
        Socket client = new Socket(SERVER_URL, PORT);

        // Scanner to receive instructions from the server.
        Scanner scanner = new Scanner(client.getInputStream());

        // Writer to respond to the server
        PrintWriter writer = new PrintWriter(client.getOutputStream(), true);

        while (scanner.hasNext()) {
            String nextLine = scanner.nextLine();
            System.out.println(nextLine);

            if (nextLine.length() == 20) {
                String result = decodeHexAndHashAndEncode(nextLine);
                System.out.println(result);
                writer.println(result);
            }
        }

        // Good programming practice - close the connection after finished.
        scanner.close();
        writer.close();
        client.close();
    }

    private static String decodeHexAndHashAndEncode(String hex) {
        byte[] bytes = HexBin.decode(hex);
        byte[] hashed = hashing.digest(bytes);

        return HexBin.encode(hashed).toLowerCase();
    }
}
