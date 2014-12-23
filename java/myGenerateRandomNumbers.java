import java.util.Random;
import java.security.*;

public class myGenerateRandomNumbers {
        public static void main(String[] args) {
                try {
                        SecureRandom random = new SecureRandom();
                        String randomInteger;

                        for (int i=0; i<1000000; i++) {
                                randomInteger = new Integer(random.nextInt()).toString();
                                System.out.println(randomInteger);
                        }
                } catch (Exception e) {
                        System.err.println(e);
                }
        }
}
