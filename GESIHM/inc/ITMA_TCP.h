/*E*/
/*  Fichier : $Id: ITMA_TCP.h,v 1.7 2017/08/24 11:09:25 devgfi Exp $        Release : $Revision: 1.7 $        Date : $Date: 2017/08/24 11:09:25 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MTCP * FICHIER ITMA_TCP.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fonctions "Coup de poing".
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	09/10/2007 : Creation  1.1
* JPL	24/10/2007 : Ajout autoroute, Pr, sens aux donnees Tunnels 1.2
* JPL	01/02/2008 : Simple changement de nom des deux phases 1.3
* JPL	23/06/2008 : Ajout indicateur 'renove' aux donnees equipements (DEM 783) 1.4
* JPL	09/01/2013 : Ajout champ 'conditions' aux donnees tunnels et cte. pour bouchon (DEM 1060)  1.5
* JPL	18/09/2013 : Ajout phase 3, echangeurs de deviation, support bidirectionnel (DEM 1083)  1.6
* JPL	18/09/2013 : Ajout variable systeme : liste des noms de tous les equipements configures (pour trace)  1.7
-------------------------------------------------------------------------------
*/


/*A
 * Definition des constantes
 * -------------------------
 */

/*A VALEUR DE VARIABLE D'ENVIRONNEMENT DEFINISSANT LE MODE COUP DE POING REEL */
/*  ------------------------------------------------------------------------- */

DEFINE	TCP_COUP_POING_REEL	"REEL"
DEFINE	TCP_COUP_POING_SIMULE	"SIMU"



DEFINE	TCP_TRACE_COUP_POING	"coup_de_poing"			' Racine du nom du fichier de trace

DEFINE	TCP_COND_BOUCHON	"BOUCHON"



/*A
** Definition des noms des variables globales de configuration
** -----------------------------------------------------------
*/

DEFINE	vg_mode_coup_de_poing	"vg_mode_coup_de_poing"		' Definit le mode reel ou trace seulement

DEFINE	vg_coup_poing_uni	"vg_cdp_s"			' Variable config. CdP unidirectionnel
DEFINE	vg_coup_poing_bidi	"vg_cdp_bidi_s"			' Variable config. CdP bidirectionnel

DEFINE	vg_sequences_bidi	"vg_seq_bidi"			' Liste des sequences CdP en bidirectionnel, sens confondus

DEFINE	vg_coup_poing_eqts	"vg_cdp_eqts"			' Liste des noms de tous les equipements (pour trace)



/*A
** Description des donnees equipements et commandes sur ces equipements
** --------------------------------------------------------------------
*/

FORMAT	TCP_Equipement
	type,						' Identifiant du type d'equipement
	renove,						' Indicateur de renovation de l'equipement
	nom,						' Nom de l'equipement
	numero,						' Identifiant de l'equipement
	sens,						' Sens de localisation de l'equipement
	echangeur,					' Echangeur de localisation de l'equipement
	cmd_phase1,					' Commande a executer en phase 1
	cmd_phase2,					' Commande a executer en phase 2
	cmd_phase3					' Commande a executer en phase 3



/*A
** Description des donnees tunnels et listes d'actions pour ces tunnels
** --------------------------------------------------------------------
*/

DEFINE	TCP_NUMERO_TUNNEL	0

FORMAT	TCP_Tunnel
	numero,						' Numero du tunnel (dans un sens donne)
	conditions,					' Conditions d'application de la sequence (bouchon...)
	num_sequence,					' Numero d'ordre de la sequence en configuration (dans un sens donne)
	nom,						' Nom du tunnel
	autoroute,					' Localisation
	sens,						'    du debut
	PR,						'       du tunnel
	ech_deviation,					' Numero d'echangeur de deviation
	ech_deviation_inv,				' Numero d'echangeur de deviation en sens inverse
	FORMAT TCP_Equipement actions			' Liste des commandes (signalisation ou neutralisation)



/*A
** Description des donnees identifiant une sequence Coup de Poing en bidirectionnel
** --------------------------------------------------------------------------------
*/

FORMAT	TCP_Sequence_Bascule
	numero_tunnel,					' Numero du tunnel objet de la sequence Coup de Poing
	numero_sequence,				' Numero d'ordre de la sequence en configuration (dans un sens donne)
	couple_ITPC					' Libelle du couple d'ITPC decrit en configuration
