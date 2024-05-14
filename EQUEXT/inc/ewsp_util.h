/*E*/
/*Fichier :  $Id: ewsp_util.h,v 1.2 2022/03/30 16:04:11 pc2dpdy Exp $      Release : $Revision: 1.:1 $        Date : $Date: 2022/03/30 16:04:11 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ewsp_util.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	30/03/22	 creation AVA 1.1
------------------------------------------------------ */

#ifndef ewsp_util
#define ewsp_util

/* fichiers inclus */
#include "xdy.h"
#include "xdm.h"
#include <xdc.h>
#include <xdg.h>
/* definitions de constantes exportees */

/* definition de macro exportees */
#define rtrim(c) \
	int q;q=strlen(c); while(q>1 && c[--q] == ' ') c[q]='\0'
/*	int q;q=strlen(c); while(q>1 && c[--q] == ' ') c[q]='\0';if (c[0]==' ') c[0]='\0'*/

/* declarations de variables globales */

/* delaration de fonctions externes */
int genererReveil(int va_numero, int va_cle, char *va_texte, int va_site_reveil);
#endif
