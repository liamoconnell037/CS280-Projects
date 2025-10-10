import java.util.Scanner;
public class Main {
    public static int getInput(String question) {
        Scanner scanner = new Scanner(System.in);
        while(true) {
            try {
                System.out.print(question);
                return Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("\nPlease type a number. \n");
            }
        }
    }
    public static void main(String[] args) {
        int response = getInput("Enter a number 1-10: ");
        System.out.println("YOU TYPED IN: " + response);
    }
}