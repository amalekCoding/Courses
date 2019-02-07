<?php
session_start();
include("menu.php");
include("connexion_bd.php");
?>

<!DOCTYPE html>
<html>
    <head>
		
        <meta charset="utf-8">
        <title>Convocation</title>
        <link rel="stylesheet" href="style.css">

		<style type="text/css">
			body {
				background-color:#ffd;
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table { margin:10px auto; border-collapse:collapse; border:1px solid gray; }
						td,th { border:10px solid gray; text-align:center; padding:20px; width: 500px;}

		</style>
    </head>
    
    <body>
    
		<h2></h2>



		<section>
			<table>
				<tr><?php
					
					echo '<tr><th>Choissiez une session</th></tr>';

					
					
					
					

					$numpersonne = $_SESSION['numpersonne'];
					
					
					
					$res = $cnx->query("SELECT idsession ,nom FROM session ORDER BY datedeb DESC");
							while($ligne = $res->fetch()){ 
								echo '<tr><td><a href="list_pers_candidat.php?idses='.$ligne['idsession'].'">'.$ligne['nom'].'</a></td></tr>';

							}
					
					

					


					?>
				</tr>
				<tr>


				</tr>
			</table>
		</section>
		
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>



