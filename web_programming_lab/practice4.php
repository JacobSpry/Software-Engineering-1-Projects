<html>
<head>
	<style>
		table, th, td {
			border: 1px solid black;
			border-collapse: collapse;
			padding: 5px;
		}
	</style>
</head>
<body>
    <a href="practice4.html">Practice four input</a> <br>

    <?php
    $size = $_POST["size"];

    echo "<h2>Multiplication Table for $size</h2>";

    echo "<table>";

    echo "<tr><th></th>";
    for ($col = 1; $col <= $size; $col++) {
        echo "<th>$col</th>";
    }

    echo "</tr>";

    for ($row = 1; $row <= $size; $row++) {
        echo "<tr>";
        echo "<th>$row</th>";

        for ($col = 1; $col <= $size; $col++) {
            echo "<td>" . ($row * $col) . "</td>";
        }

        echo "</tr>";
    }

    echo "</table>";
    ?>

</body>
</html>
