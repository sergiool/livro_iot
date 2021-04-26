<html><body>
<form action=listacess.php method=POST>
		 <p>Data Inicial: <input type=datetime-local name=DataI > 
		    Data Final: <input type=datetime-local name=DataF </p>
		 <p><input type=submit /></p>
		</form>
<?php

if ($_POST['DataI']){
   $servername = "mysql.hostinger.com.br";
   $username = "u311824763_iot";
   $password = "kedma256";
   $dbname = "u311824763_iot";
   
   // Create connection
   $conn = new mysqli($servername, $username, $password, $dbname);
   // Check connection
   if ($conn->connect_error) {
       die("Connection failed: " . $conn->connect_error);
   } 
   
   $sql = "SELECT Usuarios.Nome, Acessos.DataHora
   	   FROM Usuarios, Acessos
	   WHERE Acessos.DataHora > '" . $_POST['DataI'] . "' AND 
	         Acessos.DataHora < '" . $_POST['DataF'] . "' AND
		 Acessos.tag = Usuarios.TAG";

   $result = $conn->query($sql);
   
   if ($result->num_rows > 0) {
       // output data of each row
       echo "<TABLE><tr><th>Nome<th>Data e Hora"; 	
       while($row = $result->fetch_assoc()) {
           echo "<tr><td>" . $row["Nome"] . "<td>" . $row["DataHora"];
       }
   } else {
       echo "0 results";
   }
   $conn->close();
}

?>


		
</html></body>