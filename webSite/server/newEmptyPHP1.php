<?php

$db_hostname = 'sql2.freemysqlhosting.net';
$db_database = "sql2105932";
$db_username = "sql2105932";
$db_password = "nY9%tF2!";

$conn = new mysqli($db_hostname, $db_username, $db_password, $db_database);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

function sanitizeMYSQL($conn, $var) {

    return $var;
}

if (isset($_POST["type"])) {
    $type =  $_POST["type"];

    $returned_value = ""; //default value

    switch ($type) {
        case "tasks":
            $returned_value = display_tasks($conn);
            break;
    }
    
    echo $returned_value;
}

function display_tasks($conn) {
    $query = "SELECT * FROM `Site`;";
    $result = mysqli_query($conn, $query);
    $html = "";
    $final_result = array();
    if ($result) {
        $row_count = mysqli_num_rows($result);

        for ($i = 0; $i < $row_count; ++$i) {

            $row = mysqli_fetch_array($result);
            

            $task = array("id" => $row["ID"], "av" => $row["availability"], "lng" => $row["lng"], "lat" => $row["lat"], "desc" => $row["description"]);

            $final_result["tasks"][] = $task;
        }
    }
    
    return json_encode($final_result);
}

?>