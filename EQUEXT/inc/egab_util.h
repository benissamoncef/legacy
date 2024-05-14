/*E*/
/*Fichier :  $Id: egab_util.h,v 1.2 2018/10/26 16:04:11 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/26 16:04:11 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_util.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/01/18	 creation GABEB 1.1
* JMG	26/10/18 ajout purgerReveil et echangerFerme 1.2
------------------------------------------------------ */

#ifndef egab_util
#define egab_util

/* fichiers inclus */
#include "xdy.h"
#include "xdm.h"
#include <xdc.h>
#include <xdg.h>
#include "egab_cfg.h"
#include "egab_trace.h"
/* definitions de constantes exportees */

/* definition de macro exportees */
#define rtrim(c) \
	int q;q=strlen(c); while(q>1 && c[--q] == ' ') c[q]='\0'
/*	int q;q=strlen(c); while(q>1 && c[--q] == ' ') c[q]='\0';if (c[0]==' ') c[0]='\0'*/

/* declarations de variables globales */

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  recherche d'un CAB dans une liste chain�e de CAB
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_cab(XDY_Eqt va_numEquipement, T_CAB **pa_result);
/*
* ARGUMENTS EN ENTREE :
*  XDY_Eqt : numero d'�quipement
*  T_Liste_PRV* : liste chain�e des PRV
*
* ARGUMENTS EN SORTIE :
*  T_PRV* : pointeur vers le PRV trouv�. NULL si non trouv�
*
* CODE RETOUR :
*  XDC_OK : un PRV a �t� trouv�
*  XDC_NOK : aucun PRV trouv�
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche du PRV qui a le num�ro d'equipement pass� dans en parametre
*
------------------------------------------------------*/

int genererAlerte(T_Liste_ECH *pa_ech, T_Bouchon va_bouchon, int va_mode,
                        XDY_Octet va_sous_type_alerte, char *va_texte);
int terminerFMC(T_Bouchon va_bouchon, T_Liste_ECH *pa_ech);
int genererReveil(int va_numero, int va_cle, char *va_texte, int va_type, XDY_Eqt va_ech, XDY_Eqt va_cab); 
char *egab_NomAuto(int);
int purgerReveil(int va_numero, int va_cle);
int egab_echangeur_ferme(int numero, int mode);
#endif
