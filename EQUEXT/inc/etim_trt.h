/*E*/
/* Fichier : $Id: etim_trt.h,v 1.2 1995/05/23 12:49:59 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 12:49:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETIM * FICHIER etim_trt.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier gerant les traitements
*  de la tache TETIM
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef etim_trt
#define etim_trt

/* fichiers inclus */
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "etim_don.h"
#include "etim_sup.h"
#include "etim_ins.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction determinant la nature de la demande formulee a la tache TETIM
*  par les taches clientes.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void et_analyse_dem ( int	va_NumSock );

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket active
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Lecture du message sur la socket appelante
*   Examen du message ( programation d'un reveil ou annulation d'un reveil )
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction enregistant une demande de reveil d'une tache cliente
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void et_prog_reveil ( 	int	va_NumSock,
				char	*pa_Msg );

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket active
*   pa_Msg		: Message contenant les parametres du reveil
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Inhibition du signal d'alarme
*   Si horadate nulle : lire l'heure courante et ajouter le delai
*   Insertion de la demande de reveil dans la liste des demandes
*   Appel de la fonction et_reveiller_signal pour traiter les echeances
*   Retablissement du signal d'alarme
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction reveille les taches clientes a l'heure programmee
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void		et_reveiller_signal ( int va_signal );

/*
* ARGUMENTS EN ENTREE :
*   va_signal
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Inhibition du signal d'alarme
*   Lecture de l'horodate actuelle
*   Pour chaque enregistrement de la liste si l'horodate est echu :
*      reveil de la tache concernee
*      suppression dans la liste
*      si reveil cycle alors programation du prochaine reveil
*   Retablissement du signal d'alarme
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction supprime les demandes de reveil
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void et_annul_reveil ( 	int	va_NumSock,
				char	*pa_Msg );

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket active
*   pa_Msg		: Message contenant les parametres du reveil
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Inhibition du signal d'alarme
*   Appel de la fonction es_sup_liste
*   Appel de la fonction et_reveiller_signal pour traiter les tempos echues
*   Retablissement du signal d'alarme
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction enregistant une demande de synchro date d'une tache cliente
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void et_reveil_synchro ( 	int	va_NumSock,
					char	*pa_Msg );

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket active
*   pa_Msg		: Message contenant les parametres du reveil
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Calcul de l'horodate de reveil
*   Insertion de la demande de reveil dans la liste des demandes
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction envoie un message de reveil a la tache concerne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void et_envoyer ( ETIM_ENRG_TIMER *pa_Enr );

/*
* ARGUMENTS EN ENTREE :
*   pa_Enr		: Enregistrement arrivee a echeance
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Construction du message de reveil avec la cle
*   Appel de la fonction ex_env_msg pour envoyer le message de reveil a la tache traitement
*
------------------------------------------------------*/

#endif
