<?php
session_start();

if (!isset($_SESSION['login']) || $_SESSION['type'] != 'correcteur'){									
	header('Location: index.php'); 					/* On autorise que les correcteurs */
	die();
} 



include("menu.php");
include("connexion_bd.php");
?>


<!DOCTYPE html>
<html>
 
 <head>

<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
 

  <meta name="title" content="Calculatrice" />
  <meta name="description" content="Permet de calculer"/>
  <meta name="keywords" content="mots-clé1, mots-clé2, ..."/>
 

  <style type="text/css" media="screen">
  			body {
				background-color:#ffd;
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table { margin:10px auto; border-collapse:collapse; border:1px solid gray; }
			td,th{ border:1px solid gray; text-align:center; padding:20px; }
			h2 { border:10px solid gray; text-align:center; padding:20px; width: 500px;  margin-left: 520px;}
			input.bou {
				margin-left: 680px;
				width: 100px;
				padding: 12px 20px;

			 
				box-sizing: content-box;
				float : center;
			}	
</style>
<!--
 	 
 -->
 </head>
 
 
 <body>
 


	<h2>Crée une nouvelle Session : </h2>
  
  	
<form method= "POST" />
	<section>	
		<table>
			<tr>
				<th colspan="5">Choisir une date de debut</th>
			</tr>

			<tr>
				<td>
				<select name="jour">
							
					<?php 
					for ($i = 1; $i <= 9; $i++)
						 echo '<option value="0'.$i.'">0'.$i.'</option>';
					for ($i = 10; $i <= 31; $i++)
						 echo '<option value="'.$i.'">'.$i.'</option>';
					?>
				</select>
				</td>
				
				<td>
				<select name="mois">
					 <option value="01"> Janvier</option>
					 <option value="02"> Février</option>
					 <option value="03"> Mars</option>
					 <option value="04"> Avril</option>
					 <option value="05"> Mai</option>
					 <option value="06"> Juin</option>
					 <option value="07"> Juillet</option>
					 <option value="08"> Août</option>
					 <option value="09"> Septembre</option>
					 <option value="10"> Octobre</option>
					 <option value="11"> Novembre</option>
					 <option value="12"> Décembre</option>
				</select>
				</td>	
				
				<td>					 
				<select name="annee">
					<?php 
					for ($i = 2017; $i <= 2020; $i++)
						 echo '<option value="'.$i.'">'.$i.'</option>';
					?>
				</select>
				</td>
			</tr>
		</table>
		 </section>

			
			
			
		<section>
			<table>
				<tr>
					<th colspan="5">Choisir 10 Modules</th>
				</tr>
				<tr>
					<form method= "POST" action="">
								
							<?php
							$resultats = $cnx->query('SELECT idmodule, libelle FROM modulee WHERE idmodule NOT IN (SELECT idmodule FROM module_supp)');

							while($ligne = $resultats->fetch()){ 
								echo '<tr><td><input type="checkbox" name="choix[]" value='.$ligne['idmodule'].'></td><td>' .$ligne['libelle'].'</td></tr>';

							}
							?>

					

				</tr>
			</table>
		</section>
			
			

 
 
 
			<input class = 'bou' type="submit" value="Ajouter">
			<input class = 'bou' type="reset" value="Annuler">
				


			</form>


			 <?php 
			 if(isset($_POST['submit'])){
				 echo 'aaaa';
				 }
			 ?>
 
			 <?php
											
																			
											
											
			  if(isset($_POST) && !empty($_POST)){
                switch (count($_POST['choix'])){
					case 10:     $date = $_POST['annee']."-".$_POST['mois']."-".$_POST['jour'];
					
					
							      if($_POST['mois'] == "01") $noom = "Janvier";
							 else if($_POST['mois'] == "02") $noom = "Fevrier";
							 else if($_POST['mois'] == "03") $noom = "Mars";
							 else if($_POST['mois'] == "04") $noom = "Avril";
							 else if($_POST['mois'] == "05") $noom = "Mai";
							 else if($_POST['mois'] == "06") $noom = "Juin";
							 else if($_POST['mois'] == "07") $noom = "Juillet";
							 else if($_POST['mois'] == "08") $noom = "Aout";
							 else if($_POST['mois'] == "09") $noom = "Septembre";
							 else if($_POST['mois'] == "10") $noom = "Octobre";
							 else if($_POST['mois'] == "11") $noom = "Novembre";
							 else if($_POST['mois'] == "12") $noom = "Decembre";
					

								$nomses = $noom.' '.$_POST['annee'];
								echo '<br>';
								
								
								$insert = $cnx->query("INSERT INTO session (nom, datedeb) VALUES('".$nomses."','".$date."')");
								if(!$insert){
									echo "<script type='text/javascript'>alert('Erreur. Cette session est déjà existante');</script>";
								}
								else{
									foreach ($_POST['choix'] as $mod) //tu passes la valeur actuelle dans une clef
									 {
										$req = $cnx->query("SELECT idsession FROM session WHERE nom = '".$nomses."'");
										$idsess = $req->fetch();
										
										
										$req = $cnx->query("INSERT INTO contient (idsession, idmodule) VALUES(".$idsess[0].",".$mod.")");

										echo 'a'.$mod;
										echo '<br>';
									}
									
									
									
									
									
									
								  echo '<br>';
									$j = $_POST['jour'];
									$m = $_POST['mois'];
									$a = $_POST['annee'];
									$h ='08';

									$i = 0;
										
														  echo '<br>';
									
									$resultats = $cnx->query('SELECT m.idmodule, m.libelle FROM modulee m JOIN contient c ON c.idmodule= m.idmodule WHERE c.idsession ='.$idsess[0]);
									while($ligne = $resultats->fetch()){
									
										$date = "'".$a."-".$m."-".$j." ".$h.":00'";					
														

									
									
										
								 $cnx->query("INSERT INTO epreuve(idsession, idmodule, dateheure) VALUES(".$idsess[0].",".$ligne['idmodule'].",".$date.")");
										
										
										
										echo '<br>';
										$h = $h + 2;
										if($h == 12){
											$h= $h +2;
										}
										if($h == 18){
											$j = $j + 1;
											if($j == '32'){
												$j = 1;
												$m = $m + 1;
												if($m == '13'){
													$m = 1;
													$j = 1;
													$a = $a + 1;
												}
											
												if($m < 10){
													$m = '0'.$m;
												}	}
											if($j < 10){
												$j = '0'.$j;
											}
											$h = '08';
										 }
										 $i++;
									
									}
											 echo "<script type='text/javascript'>document.location.replace('recap_session_cree.php?idsess=+".$idsess[0]."');
								 </script>";
								}
								
								
								$ar = 2;
						
						

														
							break;

                    default: 	// si l'utilisateur coche < ou > à 10 cases
						echo "<script type='text/javascript'>alert('Veuillez coché 10 modules afin de créer une nouvelle session');</script>";

									break;
                    }
                }
                
                

				  

			  ?> 
 
 		<?php
			include("footer.php");
			
			

			
		?>
 
	</body>
</html>












