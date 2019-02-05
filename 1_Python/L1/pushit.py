from upemtk import *
from time import *
from tkinter import *


def dessin_plateau(i, j, k, n, lst, lb, hb, cpt, last_touche):
	for x in range(len(lst[0])) :														# Dessine le plateau 
		for y in range(len(lst)) :
			for l in range(int(lst[x][y])+1):
				affiche_bloc(x, y, l, lb, hb)
				if x == i and y == j and int(k) == int(lst[x][y]):
					affiche_perso(i, j, k, lb, hb, n, last_touche)
	
	affiche_bloc(len(lst)-1, len(lst)-1, int(lst[x][y]), lb, hb,  c="green")			# Dessine la case d'arrivée en vert
	
	

def affiche_bloc(i, j, k, lb, hb, c="white"):
	
	# calcul des coordonnées du coin bas du bloc
	x, y = coin_bas(i, j, k, lb, hb)
	
	
	# calcul des coordonnées des autres sommets inférieurs du bloc
	xg, xd, ymb = x - lb, x + lb, y - lb//2
	
	# calcul des ordonnées des sommets supérieurs
	ybh, ymh, yhh = y - hb, y - lb//2 - hb, y - lb - hb

	# dessin de la face supérieure, en vert si c'est l'arrivée
	face_haut = [(x, ybh), (xd, ymh), (x,  yhh), (xg, ymh)]
	polygone(face_haut, remplissage=c, epaisseur=2, tag='plateau')
		# dessin des faces latérales si hauteur non nulle
	if k > 0:
		face_gauche = [(x, y),   (xg, ymb), (xg, ymh), (x,  ybh)]
		face_droite = [(x, y),   (xd, ymb), (xd, ymh), (x,  ybh)]
		polygone(face_gauche, remplissage='#2980b9', tag='plateau')
		polygone(face_droite, remplissage='#e74c3c', tag='plateau')


def coin_bas(i, j, k, lb, hb):
	"""
	Cette fonction calcule les coordonnées, en pixels, du coin le plus bas 
	d'un bloc représenté par le triplet (i, j, k), où i est le numéro de 
	ligne et j le numéro de colonne de la case sur laquelle est posé le bloc, 
	et k est sa hauteur. Elle reçoit également les dimensions lb et hb d'un 
	bloc. 
	"""
	x = 300 + (j-i) * lb
	y = 300 + (j+i) * lb//2 - (k-1) * hb + lb
	return x, y
	
	

def affiche_perso(i, j, k, lb, hb, n, last_touche):
	"""
	Cette fonction affiche le personnage aux coordonnées (i, j, k). Elle reçoit 
	également les dimensions lb et hb d'un bloc ainsi que la taille n du 
	plateau. 
	"""
	k = int(k) + 1
	# dessin du personnage proprement dite
	x, y = coin_bas(i, j, k, lb, hb)
	
	if last_touche == 'r' or last_touche == 'u' or last_touche == '' : 											# Change la direction du personnage en le pivotant
		image(x, y - 2*lb//3, 'perso.gif', tag = 'bille')
		
	elif last_touche == 'd' or last_touche == 'l' :																# aussi
		image(x, y - 2*lb//3, 'perso2.gif', tag = 'bille')
		
	
	# repère vertical pour une meilleure visibilité
	ligne(x, y - 2*lb//3, x, 20, couleur='red', tag= 'bille')
		
		
	# flèche-repère de gauche
	x, y = coin_bas(n-1, j-0.5, 1, lb, hb)
	fleche(x - 20, y + 20, x - 10, y + 10,
		   couleur="red", epaisseur=3, tag= 'bille')

	# flèche-repère  de droite
	x, y = coin_bas(i-0.5, n-1, 1, lb, hb)
	fleche(x + 20, y + 20, x + 10, y + 10,
		   couleur="red", epaisseur=3, tag= 'bille')


def touche_ou_pas():
	# Permet de savoir si on a appuyer sur une touche
	evenement = donne_evenement()
	type_ev = type_evenement(evenement)
	if type_ev == 'Touche':
		return touche(evenement)
	else: 
		return 'pas touche'
	
def jouer(i, j, k, lst_t, lst, nom_touche, cpt, lst2, n, lb, hb, last_touche) :
	i,j, k, lst_t, cpt, lst2 = changedir(i,j,k, lst_t, lst, nom_touche, cpt, lst2)
	if len(lst_t) > 0 :			 														
			last_touche = lst_t[len(lst_t)-1]																		# 	Lui attribue la derniere touche effectué
	efface('bille')	
	efface('plateau')
	dessin_plateau(i, j, k, n, lst, lb, hb, cpt, last_touche)
	mise_a_jour()
	
	return i,j, k, lst_t, cpt, lst2, last_touche
	
def changedir(i,j,k, lst_t, lst, nom_touche, cpt, lst2):
	if nom_touche == 'Right' :
		if  j < len(lst)-1 :
			j += 1
			if k >= int(lst[i][j]) :																		# Permet de faire bouger le personnage a droite
				k = lst[i][j]	
				lst_t.append('r')
				lst2.append(' ')
				cpt += 1
				
			elif k+1 == int(lst[i][j]) and j < len(lst)-1 :	
	
				if int(lst[i][j-1]) == int(lst[i][j+1]) or int(lst[i][j]) > int(lst[i][j+1]) :				# Permet de faire bouger le cube sur la meme hauteur
					lst[i][j+1] = int(lst[i][j+1]) +1
					lst[i][j] = int(lst[i][j]) - 1
					lst_t.append('r')
					lst2.append('r') 
					cpt += 1
					

				else :																						# Si y'a un deuxieme cube derriere, le cube ne bouge pas				
					j -= 1
					k = lst[i][j]
			else :
				j -= 1
				k = lst[i][j]
		else :
			return(i,j, k, lst_t, cpt, lst2)
			

	elif nom_touche == 'Left':
		if  0 < j :
			j -= 1
			if k >= int(lst[i][j]) :																		# Permet de faire bouger le personnage a gauche
				k = lst[i][j]
				lst_t.append('l')
				lst2.append(' ')
				cpt += 1
				
			elif k+1 == int(lst[i][j]) and 0 < j :	
				if int(lst[i][j+1]) == int(lst[i][j-1]) or int(lst[i][j]) > int(lst[i][j-1]) :				# Permet de faire bouger le cube sur la meme hauteur ou de le faire tomber
					lst[i][j-1] = int(lst[i][j-1]) +1
					lst[i][j] = int(lst[i][j]) - 1
					lst_t.append('l')
					lst2.append('l')
					cpt += 1

				else :																						# Si y'a un deuxieme cube derriere, le cube ne bouge pas
					j += 1
					k = lst[i][j]
				
			else :
				j += 1
				k = lst[i][j]
		else :
			return(i,j, k, lst_t, cpt, lst2)
			
			
	elif nom_touche == 'Up' :
		if  0 < i :
			i -= 1
			
			if int(k) >= int(lst[i][j]) :																		# Permet de faire bouger le personnage en haut
				k = lst[i][j]
				lst_t.append('u')
				lst2.append(' ')
				cpt += 1
				
			elif k+1 == int(lst[i][j]) and 0 < i :	
				if int(lst[i+1][j]) == int(lst[i-1][j]) or int(lst[i][j]) > int(lst[i-1][j]) :				# Permet de faire bouger le cube sur la meme hauteur ou de le faire tomber
					lst[i-1][j] = int(lst[i-1][j]) + 1
					lst[i][j] = int(lst[i][j]) - 1
					lst_t.append('u')
					lst2.append('u')
					cpt += 1
					
				else :																						# Si y'a un deuxieme cube derriere, le cube ne bouge pas
					i += 1	
					k = lst[i][j]
			
			else :
				i += 1
				k = lst[i][j]
		else :
			return(i,j, k, lst_t, cpt, lst2)
			
			
	elif nom_touche == 'Down' :
		if  i < len(lst)-1 :
			i += 1
			
			if k >= int(lst[i][j]) :																		# Permet de faire bouger le personnage en bas
				k = lst[i][j]
				lst_t.append('d')
				lst2.append(' ')
				cpt += 1
				
			elif k+1 == int(lst[i][j]) and i < len(lst)-1 :	
				if int(lst[i-1][j]) == int(lst[i+1][j]) or int(lst[i][j]) > int(lst[i+1][j]):				# Permet de faire bouger le cube sur la meme hauteur
					lst[i+1][j] = int(lst[i+1][j]) + 1
					lst[i][j] = int(lst[i][j]) - 1
					lst_t.append('d')
					lst2.append('d')
					cpt += 1
					
				else :																						# Si y'a un deuxieme cube derriere, le cube ne bouge pas
					i -= 1
					k = lst[i][j]
			
			else :
				i -= 1
				k = lst[i][j]
		else :
			return(i,j, k, lst_t, cpt, lst2)
	
	
	elif nom_touche == 'A' :																				# Permet d'annuler la derniere action
		if len(lst_t) > 0 :
			a = lst_t.pop()
			b = lst2.pop()
			if a == 'r'  :
				j -= 1
				k = lst[i][j]
				if b == 'r' and len(lst2) > 0 :																# Permet de remettre le cube a sa place	
					lst[i][j+1] = int(lst[i][j]) + 1
					lst[i][j+2] = int(lst[i][j+2]) - 1
				else:
					pass
					
			elif a == 'l' :	
				j += 1
				k = lst[i][j]
				if b == 'l'and len(lst2) > 0  :																# Permet de remettre le cube a sa place	
					lst[i][j-1] = int(lst[i][j-1]) + 1 
					lst[i][j-2] = int(lst[i][j-2]) - 1
				else:
					pass
					
			elif a == 'u' :	
				i += 1
				k = lst[i][j]
				if b == 'u' and len(lst2) > 0 :																# Permet de remettre le cube a sa place	
					lst[i-1][j] = int(lst[i-1][j]) + 1
					lst[i-2][j] = int(lst[i-2][j]) - 1
				else:
					pass
					
			elif a == 'd' :	
				i -= 1
				k = lst[i][j]
				if b == 'd' and len(lst2) > 0 :
					lst[i+1][j] = int(lst[i+1][j]) + 1
					lst[i+2][j] = int(lst[i+2][j]) - 1
				else:
					pass
			
	
	elif nom_touche == 'N' :																				# Permet de passer au niveau suivant avec la touche N
		i = len(lst)-1
		j = i
		k = int(lst[i][j]) 
		mise_a_jour()

		
	elif nom_touche == 'Q' :																				# Permet de quitter le jeu avec la touche 'Q'
		ferme_fenetre()
			
	return(i,j, k, lst_t, cpt, lst2)
	
	
def jeu(niv) :
	lst = []																						# lst = Liste stockant la matrice
	lst2 = [' ']																					# lst2 = Liste stockants les cases bougé
	lst_t =  ['']																					# lst_t = Liste stockants les touches
	i, j, cpt, last_touche = 0, 0, 0, 0
	
	message = "R : Retry \nA : Annuler  \nP : Previous \nN : Next \nQ: Quit "
	text = texte(500, 100, message, ancrage= "center")
	
	chainee = "Niveau : " + str(niv + 1) + "/6"
	texte(99, 550, chainee, couleur="blue", ancrage=CENTER)

	niveau = "maps/tiny_map"+ str(niv) +".txt"														# Ouvre les differents niveaux
 
	file = open(niveau, "r")																		# Ouvre les fichiers des differents niveaux et integre la matrice ds une liste 'lst'
	fichier = file.readlines() 
	
	for ligne in fichier :
		matrice = ligne.split()
		lst.append(matrice)
		

	n = len(lst)
	maxi = 0
	
	for a in range(len(lst)) : 																		# Permet de trouver la plus grande valeur de la liste
		if maxi < int(max(lst[a])) :
			maxi = int(max(lst[a]))
		else : 
			pass
	
	lb = 280 / n
	hb = min(1.5 * lb, 230/(maxi + 1))

	k = int(lst[0][0])
	dessin_plateau(i, j, k, n, lst, lb, hb, cpt, last_touche)
	debut = time()
	
	return lst_t, i, j, n, lb, hb, k, lst, debut, cpt, niv, lst2, last_touche
	

#################################################################################################################################################################
# Fenetre d'acceuil

fenetre = Tk()
fen = fenetre
fenetre.title("Jeu Pushit")

photo = PhotoImage(file ='menu1.gif')

canvas = Canvas(fenetre,width=500, height=500)
canvas.create_image(300, 250, image=photo)
canvas.pack(side=LEFT, padx=0, pady=0)

# frame 1
Frame1 = Frame(fenetre, borderwidth=10, relief=GROOVE)
Frame1.pack(side=LEFT, padx=30, pady=30)

# frame 2
Frame2 = Frame(Frame1, borderwidth=2, relief=GROOVE)
Frame2.pack(side=BOTTOM, padx=10, pady=10)

# frame 3
Frame3 = Frame(Frame1, bg="white", borderwidth=2, relief=GROOVE)
Frame3.pack(side=BOTTOM, padx=5, pady=5)

# frame 4
Frame4 = Frame(Frame1, borderwidth=2, relief=GROOVE)
Frame4.pack(side=BOTTOM, padx=10, pady=10)

# Ajout de labels
Label(Frame1, text="Choisisser votre mode de Jeu : ").pack(padx=15, pady=20)

Label(Frame2, text="Quitter").pack(padx=25, pady=10)
Label(Frame3, text="Chargez Partie").pack(padx=8, pady=10)
Label(Frame4, text="Nouvelle Partie",bg="white").pack(padx=8, pady=10)


# boutons
b1 = Button(Frame2, text ='Cliquez ici')
b1.pack(side =RIGHT, padx =10, pady =10)

b2 = Button(Frame3, text ='Cliquez ici')
b2.pack(side =RIGHT, padx =10, pady =10)

b4 = Button(Frame4, text ='Cliquez ici', command = fen.destroy)
b4.pack(side =RIGHT, padx =10, pady =10)

 
fenetre.mainloop()

######################################################################################################################################################
# Fenetre de jeu


cree_fenetre(600, 600)

niv = 0
niv_fini = 'non'

lst_t, i, j, n, lb, hb, k, lst, debut, cpt, niv, lst2, last_touche = jeu(niv)


if __name__ == '__main__':
	while True  :
		nom_touche = touche_ou_pas()
		mise_a_jour()
		(i,j, k, lst_t, cpt, lst2, last_touche) = jouer(i, j, int(k), lst_t, lst, nom_touche, cpt, lst2, n, lb, hb, last_touche)
		
		if i == len(lst)-1 and j == len(lst)-1 and int(k) == int(lst[i][j]): 							# Permet de passer au niveau suivant si on est ds la derniere case
			niv = niv+1
			niv_fini = 'oui'
			affiche_perso(i, j, k, lb, hb, n, last_touche)

		mise_a_jour()

		if niv_fini == 'oui' :																					# Si on a gagné un niveau
			niv_fini = 'non'
			chaine = "Bravo !"
			texte(150, 50, chaine, couleur="red", ancrage=CENTER)
			chaine = "Niveau gagner en " + str(round(time() - debut, 2)) + "s"
			texte(150, 80, chaine, couleur="red", ancrage=CENTER)
			chainee = "et en " + str(cpt) + " coups"
			texte(99, 110, chainee, couleur="red", ancrage=CENTER)
			rectangle(6, 30, 303, 130, couleur="blue", epaisseur=3)
			
			if niv > 5:																							# Fin du jeu
				attente_clic()
				ferme_fenetre()
				cree_fenetre(300, 300)
				message= "Bravo ! Vous avez gagné  \ntous les niveaux"
				text = texte(10, 100, message)
				mise_a_jour()
				break
			
			attente_clic()
			ferme_fenetre()
			cree_fenetre(600, 600)
			
			lst_t, i, j, n, lb, hb, k, lst, debut, cpt, niv, lst2, last_touche = jeu(niv)
	 




		elif nom_touche == 'P' :																			# Permet de passer au niveau inferieure avec la touche P
			if niv > 0 : 
				niv = niv - 1
				attente_clic()
				ferme_fenetre()
				cree_fenetre(600, 600)
				lst_t, i, j, n, lb, hb, k, lst, debut, cpt, niv, lst2, last_touche = jeu(niv)
		
		elif nom_touche == 'R' :																			# Permet de remettre a 0 le niveau avec la touche A
			ferme_fenetre()
			cree_fenetre(600, 600)
			lst_t, i, j, n, lb, hb, k, lst, debut, cpt, niv, lst2, last_touche = jeu(niv)
		


	attente_clic()



