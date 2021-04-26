<?
$short_connect = new mysqli("mysql.hostinger.com.br", "u311824763_iot", "kedma256", "u311824763_iot");

$sql = "SELECT * FROM Usuarios WHERE tag = '" . $_POST[tag] . "'";
$result = $short_connect->query($sql);
if ((!$result) || ($result->num_rows == 0)){ // Tag não cadastrada 
	echo "NOTAG\n";
	$result->free();
	$sql = "INSERT INTO Insercao (TAG) VALUES ('". $_POST[tag] . "')";	
	$result = $short_connect->query($sql);
	if($result) 
		echo "NOAUTH\n";
} else { // Tag existente, registra acesso 
	echo "OK\n";
	$result->free();
	$sql = "INSERT INTO Acessos (TAG) VALUES ('". $_POST[tag] . "')";	
	$result = $short_connect->query($sql);
}
$short_connect->close();

?>