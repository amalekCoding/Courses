function afficher_cacher(id)
{
	if(document.getElementById(id).style.visibility=="hidden")
	{
		document.getElementById(id).style.visibility="visible";
		document.getElementById('bouton_'+id).innerHTML='Cacher';
	}
	else
	{
		document.getElementById(id).style.visibility="hidden";
		document.getElementById('bouton_'+id).innerHTML='Ajouter un module';
	}
	return true;
}



			function VerificationDestinataire(formulaire) {
				var cpt = 0;
				while (cpt < 10) {
				   if (document.getElementById("id_personne"+cpt).options[0].selected) {
					alert('Veuillez sélectionner pour chaque epreuve une salle');
					return false;
				   }
				cpt++;

				} 
		   return true;
		  }
