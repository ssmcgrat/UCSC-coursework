import java.sql.*;
import java.io.*;
import java.util.*;

/**
 * A sample class that connects to PostgreSQL and runs a simple query.
 *
 * Note: Your database name is your login name, so for login jsmith, you would
 * have : getConnection("jdbc:postgresql://cmps180-db.lt.ucsc.edu/jsmith",
 * "jsmith" , "password");
 */
public class Driver
{
    public static void main(String[] args) {
    	
    	Connection connection = null;
    	try {
    		//Register the driver
    		Class.forName("org.postgresql.Driver"); 
    		// Make the connection
    		connection = DriverManager.getConnection(
    				"jdbc:postgresql://cmps180-db.lt.ucsc.edu/ssmcgrat", 
    				"ssmcgrat",
    				"calculated33XML"); 

    		StoreApplication app = new StoreApplication();
    		List<String> phoneNumbers = app.getCustomerPhoneFromFirstLastName(
    				connection, 
    				"John", 
    				"Smith");

    		/************ Print the phone numbers here: ****************/

    		List<String> filmTitles = app.getFilmTitlesBasedOnLengthRange(
    				connection, 
    				60, 
    				120);
    		
    		/************* Print the film titles here: *****************/

    		int count = app.countCustomersInDistrict(
    				connection, 
    				"Buenos Aires",
    				false);

    		/************ Print the customer count here: **************/
    		
    		// add a film to the database
    		app.insertFilmIntoInventory(
    				connection, 
    				"Sequel to the Prequel",
    				"Memorable", 
    				98, 
    				"PG-13");
    	}
    	catch (SQLException | ClassNotFoundException e) {
    		System.out.println("Error while connecting to database: " + e);
    		e.printStackTrace();
    	}
    	finally {
    		if (connection != null) {
    			// Closing Connection
    			try {
					connection.close();
				} catch (SQLException e) {
					System.out.println("Failed to close connection: " + e);
					e.printStackTrace();
				}
    		}
    	}
    }
}
