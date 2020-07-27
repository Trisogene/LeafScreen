<?php

// Set DB variables
$host = "localhost";
$dbname = "dbName";   
$username = "dbUsername";
$password = "dbPassword";

// Establish connection to MySQL database
$conn = new mysqli($host, $username, $password, $dbname);

// Check if connection established successfully
if ($conn->connect_error)
    die("Connection failed: " . $conn->connect_error);

// Select values from MySQL database table
$sql = "SELECT id, value, date, time FROM table";
$result = $conn->query($sql);

// Print the value on the website
echo "<center>";
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        echo "<strong> Id:</strong> " . $row["id"]. " &nbsp <strong>value:</strong> " . $row["value"]. " &nbsp <strong>Date:</strong> " . $row["date"]." &nbsp <strong>Time:</strong>" .$row["time"]. "<p>";
    }
} else {
    echo "0 results";
}
echo "</center>";

// Close the connection
$conn->close();

?>
