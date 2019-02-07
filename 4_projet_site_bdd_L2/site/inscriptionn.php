<?php
session_start();


if (isset($_SESSION['login'])){									
	header('Location: index.php'); 					/* Si deja connecté */
	die();
} 

include("menu.php");
include("connexion_bd.php");
?>


<html>

	<style type="text/css">

	p {
	  margin-top: 0px;
	}
	 
	fieldset {
	  margin-bottom: 15px;
	  padding: 10px;
	  margin-left: auto;
	  margin-right: auto;
	  width: 500px;
	}
	 
	legend {
	  padding: 0px 3px;
	  font-weight: bold;
	  font-variant: small-caps;
	}
	 
	label {
	  width: 110px;
	  display: inline-block;
	  vertical-align: top;
	  margin: 6px;
	}
	 
	em {							/* Permet de metre en rouge les asterix*/
	  font-weight: bold;			
	  font-style: normal;
	  color: #f00;
	}
	 
	input:focus {
	  background: #eaeaea;
	}
	 
	input, textarea {
	  width: 249px;
	}
	 
	textarea {
	  height: 100px;
	}
	 
	select {
	  width: 254px;
	}
	 
	input[type=checkbox] {
	  width: 10px;
	}
	 
	input[type=submit] {
	  width: 150px;
	  padding: 10px;
	}

	input[type="password"] {
	 width: 200px;
	    margin-bottom: 10px;
		padding: 1px;
	}

			input.bou {
				margin-left: 880px;
				width: 100px;
				padding: 12px 20px;

			 
				box-sizing: content-box;
				float : center;
			}

	</style>
    <body>
	<h2>Formulaire d'inscription</h2>

	<form action="#" method = "GET">
	  <p><i>Complétez le formulaire. (Les champs marqué par </i><em>*</em> sont <em>obligatoires)</em></p>

	  <fieldset>
	    <legend>Infos Compte</legend>
	      <label for="login">Login <em>*</em></label>
	      <input name = "login" id="login" placeholder="jedi" autofocus="" required=""><br>

	      <label for="psw">Password <em>*</em> </label>
	      <input name = "psw" id="psw" placeholder="*****" type="password" required=""><br>

	      <label for="email">Email <em>*</em></label>
	      <input name = "email" id="email" placeholder="prenom.nom@etud.edu" required="" pattern="[a-zA-Z]*.[a-zA-Z]*@etud.edu" type="email"><br>
	  </fieldset>

	  <fieldset>
	    <legend>Contact</legend>
	      <label for="nom">Nom <em>*</em></label>
	      <input name = "nom" id="nom" placeholder="Moulin" autofocus="" required=""><br>

	      <label for="prenom">Prenom <em>*</em></label>
	      <input name ="prenom" id="prenom" placeholder="Jean" autofocus="" required=""><br>

	      <label for="telephone">Portable</label>
	      <input name = "tel" id="telephone" placeholder="06xxxxxxxx" pattern="06[0-9]{8}" type="tel"><br>

	  </fieldset>


	  <fieldset>
	    <legend>Je suis un : <em>*</em></legend>
		<input id ="case" type="radio" name="type" value="etudiant" onclick="activer('etudiant')" required/>Etudiant
		<input id ="case" type="radio" name="type" value="personnel" onclick="activer('personnel')" />Personnel
		<input id ="case" type="radio" name="type" value="exterieur" onclick="activer('exterieur')" />Exterieur
		
		<input id ="case" type="radio" name="type" value="surveillant" onclick="activer('surveillant')" />Surveillant
		<input id ="case" type="radio" name="type" value="correcteur" onclick="activer('correcteur')" />Correcteur
	  </fieldset>




	  <fieldset>
		<legend>Information personnelles</legend>



		<fieldset id="etudiant" disabled="disabled">

			Numero Etudiant : <input id = "num" name="numetudiant" required/><br>
			Année : <select name="Annee">



				<?php
					$resultats = $cnx->query('SELECT * FROM annee_universitaire');
					while($ligne = $resultats->fetch()){ 
						echo ' <option value='.$ligne['annee_univ'].'>'.$ligne['annee_univ'].'</option>';

					}
				?>

			</select><br>
			
			Formation : <select name="Formation">
				<?php
				$resultats = $cnx->query('SELECT * FROM formation');

				while($ligne = $resultats->fetch()){ 
					echo ' <option value='.$ligne['idformation'].'>'.$ligne['libelle'].'</option>';

				}
				?>
				</select>



	</fieldset>

	<fieldset id="personnel" disabled="disabled">
		Service administratif : <select name="Service_Admi">
				<?php
				$resultats = $cnx->query('SELECT * FROM service_admini');

				while($ligne = $resultats->fetch()){ 
					echo ' <option value='.$ligne['libelle'].'>'.$ligne['libelle'].'</option>';

				} 			
				?>crée table personelle
		</select>
	</fieldset>

	<fieldset id="exterieur" disabled="disabled">
		N°Siret : <input name="Num_Siret" required /><br>
		Nom : <input name="nomSociete" required /><br>
		Adresse : <input name="adr" required />

	</fieldset>
	
	<fieldset id="Surv_Correc" disabled="disabled">
		Num de Compte : <input name="Num_Cpt" required /><br>

	</fieldset>
	




	<script>
	function activer(id){
		var $etudiant = document.getElementById('etudiant');
		var $personnel = document.getElementById('personnel');
		var $exterieur = document.getElementById('exterieur');
		var $Surv_Correc = document.getElementById('Surv_Correc');
		if(id == 'etudiant' && $etudiant.disabled){
			$etudiant.disabled = !$etudiant.disabled;
			if(!$personnel.disabled)
				$personnel.disabled = true;
			if(!$exterieur.disabled)
				$exterieur.disabled = true;
			if(!$Surv_Correc.disabled)
				$Surv_Correc.disabled = true;
		}
		else if(id == 'personnel' && $personnel.disabled){
			$personnel.disabled = !$personnel.disabled;
			if(!$etudiant.disabled)
				$etudiant.disabled = true;
			if(!$exterieur.disabled)
				$exterieur.disabled = true;
			if(!$Surv_Correc.disabled)
				$Surv_Correc.disabled = true;
		}
		else if(id == 'exterieur' && $exterieur.disabled){
			$exterieur.disabled = !$exterieur.disabled;
			if(!$etudiant.disabled)
				$etudiant.disabled = true;
			if(!$personnel.disabled)
				$personnel.disabled = true;
			if(!$Surv_Correc.disabled)
				$Surv_Correc.disabled = true;				
		}
		else if((id == 'surveillant' || id == 'correcteur') && $Surv_Correc.disabled){
			$Surv_Correc.disabled = !$Surv_Correc.disabled;
			if(!$etudiant.disabled)
				$etudiant.disabled = true;
			if(!$personnel.disabled)
				$personnel.disabled = true;
			if(!$exterieur.disabled)
				$exterieur.disabled = true;				
		}
	};
	</script>





	  </fieldset>



<input class = 'bou' type="submit" value="Ajouter">

	</form>
	
	
	    	<?php
			
			$mdp = md5($_GET['psw']);


			$resultats = $cnx->query("INSERT INTO authentification VALUES('".$_GET['login']."', '".$mdp."' , '" .$_GET['type']. "')");
			if($_GET['type'] == 'surveillant' || $_GET['type'] == 'correcteur'){
				$a = $cnx->query("INSERT INTO surveillant_correcteur (nom, prenom, telephone, numcomptebancaire, login) VALUES('".$_GET['nom']."', '".$_GET['prenom']."' , '" .$_GET['tel']."' , '" .$_GET['Num_Cpt']."', '" .$_GET['login']."')");
				$res = $cnx->query("SELECT idsurv_cor FROM surveillant_correcteur WHERE login = '".$_GET['login']."'");

			}
			else{
				$a = $cnx->query("INSERT INTO personne (nom, prenom, mail, login) VALUES('".$_GET['nom']."', '".$_GET['prenom']."' , '" .$_GET['email']."', '" .$_GET['login']."')");
				$res = $cnx->query("SELECT numpersonne FROM personne WHERE login = '".$_GET['login']."'");
			}
			
			
			$numpersonne = $res->fetch();


			if($_GET['type'] == 'etudiant'){
				$cnx->query("INSERT INTO etudiant VALUES(".$numpersonne[0].", '".$_GET['numetudiant']."' , " .$_GET['Formation']. ",  '" .$_GET['Annee']. "')");
			}
			else if($_GET['type'] == 'personnel'){
				$cnx->query("INSERT INTO provient_service VALUES(".$numpersonne[0].", '".$_GET['Service_Admi']."')");
			}
			else if($_GET['type'] == 'exterieur'){
				$cnx->query("INSERT INTO exterieur VALUES(".$numpersonne[0].", '".$_GET['Num_Siret']."', '".$_GET['nomSociete']."', '".$_GET['adr']."')");
			/* enlever la colonne telephone */
			}

		?>
	
    </body>
</html>
