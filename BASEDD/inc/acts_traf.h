/* Fichier : acts_traf.h   Release : 1.1   Date : 05/15/97	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acts_traf.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client trafic
* pour le PC STRADA
* Il contient les definitions des fonctions
* utilisees dans acts_traf.c
*************************************************
* HISTORIQUE :
*
* PENOT	05 MAI 1997 : Creation 		V1.1
* Niepceron 	05 Jan 1998 : intergestionnaire dem/1724 v1.3
************************************************/

#ifndef CLIENT_TRAF
#define CLIENT_TRAF

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include 	<stdlib.h>
 
#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzis.h"
#include "xzic.h"
#include "xzas62.h"

#define	XINC_VITESSE_K	1
#define	XINC_VITESSE_P	2
#define	XINC_VITESSE_V	3
#define C_PATH_FTP              XDC_PATH_STRADA"/ftp/"
#define C_SCRIPT_TRAF           C_PATH_FTP"traf_script_get"


	/* structure utilisee pour stocker la localisation rds */
 
 struct code_loc {
 	int		option;
        char            l01[5];                 	/* numero de la table de localisation   		*/
        int            	l02;                 		/* code du point RDS en val le + proche ou segment   	*/
        char            l03[72];                	/* Libelle associe au code RDS    			*/
        char            l04[37];                	/* Texte libre du champ de localisation mesure		*/
        int             l05;                    	/* Distance du point RDS aval le + proche		*/
        char            l06[37];                	/* Texte libre						*/
  
        };
        typedef struct code_loc loc_rds;


/* definition de structures */

	/* structure utilisee pour stocker le code associe au equipements de STRADA_NET */

struct code_eqt{
	char *    		string;			/* nom de l'equipement Strada 		*/
  	XDY_Eqt   		number;			/* code associe au SAE        		*/
  	
  	XDY_Horodate		h_1;			/* horodate sens 1            		*/
  	int			h_1_b;			/* horodate sens 1  boolean     	*/
  	XDY_Horodate		h_2;			/* horodate sens 2            		*/
  	int			h_2_b;			/* horodate sens 2  boolean     	*/
  	
  	int 			q_1,q_2;		/* debit           	sens 1 et 2	*/
  	int 			q_f_1,q_f_2;		/* frequence       	sens 1 et 2	*/
  	int 			q_1_b,q_2_b;		/* boolean         	sens 1 et 2	*/
	int			q_1_classe,q_2_classe;	/* classe de q	   	sens 1 et 2	*/
	loc_rds			q_1_loc,q_2_loc;	/* localisation rds	sens 1 et 2	*/

  	int 			v_1,v_2;		/* vitesse         	sens 1 et 2	*/
  	int 			v_f_1,v_f_2;		/* frequence       	sens 1 et 2	*/
  	int 			v_1_b,v_2_b;		/* boolean        	sens 1 et 2	*/
	int			v_1_classe,v_2_classe;	/* classe de v	   	sens 1 et 2	*/
	loc_rds			v_1_loc,v_2_loc;	/* localisation rds	sens 1 et 2	*/

  	int 			to_1,to_2;		/* taux occupation 	sens 1 et 2	*/
  	int 			to_f_1,to_f_2;		/* frequence    	sens 1 et 2	*/
  	int 			to_1_b,to_2_b;		/* boolean t.o. 	sens 1 et 2	*/
  	int			to_1_classe,to_2_classe;/* classe de t.o.	sens 1 et 2	*/
	loc_rds			to_1_loc,to_2_loc;	/* localisation rds	sens 1 et 2	*/


  	int			e_1,e_2;	/* etat de modification de cette structure par sens	*/
  	
  	struct code_eqt* 	suivant;	/* pointeur sur l'equipement suivant	*/
	};
	typedef struct code_eqt * liste_eqt;
	
	


/* definition des constantes */

/* d≈finition des variables globales » l'application */

/* definition des variables locales au module */

/* declaration de fonctions internes */

/* definition de fonctions externes */

	/* procedure utilisee lors de l'initialisation du seveur trafic
 	  	- Programmation du timer de donees radt
  	 	- lecture du fichier de config frequence.txt
   		- initialisation de la liste contenant les codes equipement Strada
   		  dans le fichier eqt_stra.txt                                          */

   	extern int acts_traf_init();

	/* procedure permettant de recupere les fichiers trafic sur le PC Strada        */

	extern int acts_traf_ftp();
	
	/* procedure permettant d'effacer les fichiers trafic sur le PC Strada          */

	extern int acts_traf_ftpdel();
	
	extern int acts_traf_decode_readfile_update(char *pa_name, XDY_Horodate va_Horodate, XDY_Sens va_Sens, 
					XDY_Voie va_Voie, XDY_Octet va_TypeDonnees, int va_Donnees, int va_Unite, int va_Classe, int va_frequence,loc_rds pa_loc) ;

	/* procedure permettant de decoder les fichiers trafic, de verifier leur format
	   et de fournir les informations au synoptique Strada                          */
	   
	extern void acts_traf_decode();

	/* procedure permettant de liberer la memoire de la liste chainee 
	   equipement_strda lorsque la tache client va etre arretee                     */

	extern void acts_traf_freemem();


	extern void acts_export_cat();
	extern void acts_import_cat();

#endif
