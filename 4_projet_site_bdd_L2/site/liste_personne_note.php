<?php
session_start();
include("menu.php");
include("connexion_bd.php");
?>


    <head>
		
        <meta charset="utf-8">
        <title>Choix Module</title>
        <link rel="stylesheet" href="style.css">

		<style type="text/css">
			body {
				background-color:#ffd;
				font-family:Verdana,Helvetica,Arial,sans-serif;
			}
			table {  padding: 10px; margin-left: 250px; float : left;
	  
					border-collapse:collapse; border:1px solid gray; }
			td,th { border:1px solid gray; text-align:center; }
			
			table
			{
				border-collapse : collapse;
				background-color : #EBD9C8;
			}

			th,td
			{
				border : 1px solid black;
				height : 40px;
				
				float : center;
			}

			th
			{
				background-color : #D48030;
			}

			.impair
			{
				background-color : #BEA58E;
			}

				fieldset {
				  margin-bottom: 15px;
				  padding: 10px;
				  margin-left: 80px;
				  width: 200px;
				  float : left;
				}
			h3 { border:10px solid gray; text-align:center; padding:15px; width: 450px;  margin-left: 400px;}

		</style>
		

		
		<script type="text/javascript" src="aff_cacher_tab.js"></script>
    </head>
    
    <body>
		<div id="boutonsidentite" class="boutonsp">
			<button type="button" value="valider" id="valider" onmouseup=""><a class ="b" href="corrige.php"><-- RETOUR</a> </button>
			<?php echo '<a href="mescorrections.php?idses='.$_GET['idses'].'&amp;idmod='.$_GET['id_module'].'">Consulter mes corrections</a>' ?>
			<button type="button" value="creernouvellepers" id="creerpers">Voir resultats --></button>
			<br/><br/>
		</div>
    
		<h3>Liste personnes : </h3>

			<form method= "POST" action="insere_noter_candidat.php">
			  <fieldset>
				<legend>Je peux corriger : <em>*</em></legend>
				


				<?php
					$resultats = $cnx->query('SELECT DISTINCT m.libelle, m.idmodule, m.libelle FROM peut_corriger pc JOIN modulee m ON m.idmodule=pc.idmodule JOIN contient c ON c.idmodule = m.idmodule WHERE c.idsession='.$_GET['idses'].' AND pc.idcorrecteur ='.$_SESSION['numpersonne']);
					foreach($resultats  as $key => $value)
					{  						

						echo '<a href="liste_personne_note.php?id_module='.$value['idmodule'].'&amp;idses='.$_GET['idses'].'">'.$value['libelle'].'</a><br />';
						echo '<br />';

					}	
					
					$req = $cnx->query('SELECT nom FROM session WHERE idsession = '.$_GET['idses']);
					$nameses = $req->fetch();
					
					if(isset($_GET['id_module'])){
						$idmodule = $_GET['id_module'];
						$req = $cnx->query('SELECT libelle FROM modulee WHERE idmodule = '.$idmodule);
						$nommodul = $req->fetch();
					}
					else{ 
						$idmodule = 0;
					}
?>
			</fieldset>
			
			
			


		<section>
			
			<table>
				<tr>
					<th colspan="20">
						<?php
							echo $nameses['nom']. ' : <br> ' .$nommodul['libelle'];
						?>
					</th>
				</tr>

				<tr>
					<th colspan="7">Nom</th>
					<th colspan="7">Prenom</th>
					<th colspan="5">Note /20</th>

				</tr>
				
				
				<tr>			
				  <?php
				  
				  
					$numpersonne = $_SESSION['numpersonne'];
				
					$idses = $_GET['idses'];
					$id_module = $_GET['id_module'];
					
					
					if(!isset($_GET['id_module'])){
						echo '<tr><td colspan="15">Veuiller selectionez un module</td></tr>';
					}		
					else{
						echo  "<input class = 'boutt' type='submit' name='Ajouter' value='Ajouter'/><br/>";
					}

					


				  	$res = $cnx->query("SELECT codeepreuve FROM epreuve WHERE idsession =".$idses." and idmodule =".$id_module);
					if($res)
						$idepreuve = $res->fetch();
					
					$resultat = $cnx->query("CREATE VIEW a".$idses." AS SELECT numpersonne, nom, prenom FROM inscrit i JOIN personne p ON p.numpersonne = i.codepersonne WHERE codesession =".$idses);
					$resultats = $cnx->query("SELECT * FROM a".$idses." WHERE NOT EXISTS (SELECT * FROM corrige c WHERE codeepreuve = ".$idepreuve[0]." and c.numpersonne =a".$idses.".numpersonne)");
					
					$cpt_person = 0;
					if($resultats){
						while($ligne = $resultats->fetch()){
							$cpt_person  = $cpt_person + 1;

							echo '<tr>';
							
							echo '<td colspan="10">'.$ligne['nom'].'</td><td colspan="3">'.$ligne['prenom'].'</td>';
							
							?>
							
							<td  colspan="5">
								<select name="Note[]">
									<?php
									echo '<option value='.NULL.'>Note</option>';
									for ($i =0; $i <= 20; $i++){
										echo '<option value="'.$i.'">'.$i.'</option>';
									} 
									?>
								</select>
							</td>

							
					</tr>

				<?php 
					}
				}
				
				if(($cpt_person == 0) && isset($id_module)){
					echo '<tr><td colspan="15">Personne a corriger</td></tr>';
				}
		      ?>
			</tr>

				
		  </table>
			
		</section>

		 <?php if(isset($id_module)){
			 ?>
		<section>
			<table>
				<th colspan="5">Note  mini : </th>	
				<tr>
					<td>
						<?php
						$req = $cnx->query("SELECT note_min FROM contient WHERE idsession =".$idses." and idmodule =".$id_module);
						$note = $req->fetch();
						
						if(empty($note['note_min'])){
						?>	
							<select name="notemin">
								<?php
								echo '<option value='.NULL.'>Note</option>';
								for ($i =0; $i <= 20; $i++){
									echo '<option value="'.$i.'">'.$i.'</option>';
								} 
								?>            
								
							</select> 
							<?php
						}
						echo $note['note_min'].'/20';
							?>
					</td>
				<tr><td><input type="submit" name="notemini" value = "Modifier" /></td></tr>

			  </tr>	

			</table>
		</section>
		
		
		<?php  }
		if(isset($idses) && isset($id_module)){
			echo "<input name = 'session' value =".$idses." type= 'hidden' />";
			echo "<input name = 'mod' value =".$id_module." type= 'hidden' />";
		}
		?>



		</form>
		
		<?php
			include("footer.php");
		?>
		
	</body>
</html>




