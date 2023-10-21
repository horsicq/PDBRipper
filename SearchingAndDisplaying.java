import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Structure {
    private String name;
    private int id;

    public Structure(String name, int id) {
        this.name = name;
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public int getId() {
        return id;
    }
}

public class SearchExactMatch {
    public static void main(String[] args) {
        // Sample data structures (replace this with your own data)
        List<Structure> structures = new ArrayList<>();
        structures.add(new Structure("ABC", 1));
        structures.add(new Structure("XYZ", 2));
        structures.add(new Structure("PQR", 3));

        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter the name to search: ");
        String searchName = scanner.nextLine();

        // Perform the search and display exact match first
        displayResults(structures, searchName);

        scanner.close();
    }

    private static void displayResults(List<Structure> structures, String searchName) {
        // Separate exact matches and other matches
        List<Structure> exactMatches = new ArrayList<>();
        List<Structure> otherMatches = new ArrayList<>();

        // Iterate through the structures
        for (Structure structure : structures) {
            if (structure.getName().equalsIgnoreCase(searchName)) {
                // Exact match
                exactMatches.add(structure);
            } else {
                // Other matches
                otherMatches.add(structure);
            }
        }

        // Display exact matches first
        System.out.println("Exact Matches:");
        for (Structure structure : exactMatches) {
            System.out.println("Name: " + structure.getName() + ", ID: " + structure.getId());
        }

        // Display other matches
        System.out.println("Other Matches:");
        for (Structure structure : otherMatches) {
            System.out.println("Name: " + structure.getName() + ", ID: " + structure.getId());
        }
    }
}
