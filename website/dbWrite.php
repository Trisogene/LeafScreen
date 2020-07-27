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

// Get date and time variables
date_default_timezone_set('Europe/Amsterdam');
$d = date("Y-m-d");
$t = date("H:i:s");
    
// Insert value into SQL db
if(!empty($_POST['sendval']) )
{
    $val = $_POST['sendval'];
    $sql = "INSERT INTO table (value, date, time) VALUES ('".$val."', '".$d."', '".$t."')"; 
}

// Close the connection
$conn->close();

?>
