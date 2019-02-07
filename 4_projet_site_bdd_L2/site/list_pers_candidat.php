<?php
session_start();
include("menu.php");
include("connexion_bd.php");
?>



    <head>
		
        <meta charset="utf-8">
        <title>Convocation</title>
        <link rel="stylesheet" href="style.css">

		<style type="text/css">
			body {
				background-color:#ffd;
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table { margin:5px auto; border-collapse:collapse; border:1px solid gray;  margin-top:50px;}
			td,th { border:10px solid gray; text-align:center; padding:20px; width: 500px;}

		</style>
    </head>
    
    <body>
    

<form method= "GET" action="note_sess_candidat.php">
			<table border WIDTH="30%">
				<tr>
					<th>Candidat</th>
				</tr>
				<tr>
					
					
						
					
							<?php	
								$modul = $cnx->query('SELECT * FROM personne p JOIN inscrit i ON p.numpersonne = i.codepersonne WHERE i.codesession ='.$_GET['idses']);
								
 								$test = $cnx->query('SELECT count(*) FROM (SELECT * FROM personne p JOIN inscrit i ON p.numpersonne = i.codepersonne WHERE i.codesession ='.$_GET['idses'].') as b');

$testo = $test->fetch();

								if(!empty($testo[0])){
									while($ligne = $modul->fetch()){ 
										echo '<tr><td><a href="note_sess_candidat.php?idses='.$_GET['idses'].'&amp;idpers='.$ligne['numpersonne'].'">'.$ligne['nom'].' '.$ligne['prenom'].'</a></td></tr>';
									}
								}
								else{
									echo '<tr><td>Resultat non disponible</td></tr>';
								}
							?>
						
					 
						
								
				</tr>
			</table>
</form>
				
		<?php
			include("footer.php");
		?>
	</body>
</html>

