/*E*/
/*Fichier : $Id: isbi_sra_ech.c,v 1.3 2013/08/28 09:25:40 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2013/08/28 09:25:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER isbi_sra_ech.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques sur echangeur
*   
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	12 Sep 1994	: Creation
* JMG		14/03/13 : filtre canWeSend
------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sra_ech.h"
#include "isbi_sra.h"
/* definitions de constantes */

#define CM_LOC_A	1
#define CM_LOC_B	2
#define CM_LOC_C	3
#define CM_LOC_AB	4
#define CM_LOC_BC	5
#define CM_LOC_AC	6

#define CM_INC_ETAT     (0.01)
#define CM_BUFF_SOCK 600

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)isbi_sra_ech.c	1.11 12/23/96 : isbi_sra_ech" ;

XDY_Booleen vm_cx;
char vl_msg[CM_BUFF_SOCK];

/* declaration de fonctions internes */

extern int   MSRA_Existe_mob();

void MSRA_Abonnements_radt();



/* definition de fonctions externes */
extern int   MSVU_Creer_thash_vag();
extern int   MSVU_Recherche_vag();
extern int   MSVU_Affecte_vag();


extern int   MSRA_init_tabordobj();
extern int   MSRA_Tri_obj();

extern int   MSOP_Inserer_fop();
extern int   MSOP_Detruire_fop();
extern int MSOP_init_trt_actions_operateur();
extern void MSMA_arret();
extern int canWeSend(char *);
extern int MSRA_Init_Recept();

extern void MSOP_Affiche_elt_fop();
extern int send_message();


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un mobile et de l'objet 
*  associe
------------------------------------------------------
* SEQUENCE D'APPEL :
*/ 

MSRA18_raf_objets_mobiles_echang(
          va_type_mobile,
          va_typeraf,
          va_no_fmc,
          va_no_act,
          pa_nom_ech,
          va_sens,
          va_localisation)
          
          XDY_Octet va_type_mobile;
          XDY_Octet va_typeraf;
          XDY_Evt va_no_fmc;
          XDY_Evt va_no_act;
          char *pa_nom_ech;
          XDY_Sens va_sens;
          XDY_Octet va_localisation;

/*
* ARGUMENTS EN ENTREE :
*   va_typeraf : XDC_AJOUT, XDC_MODIF ,XDC_SUPPR : 
*                type de rafraichissement
*   va_no_fmc :  No de fmc
*   va_no_act :  No d'action
*   pa_nom_ech : nom de l'echangeur
*   va_localisation : point de localisation sur echangeur
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   Creation de l'objet et de sa vag de localisation
*   Modification de la vag de localisation
*   Suppression de l'objet et de sa vag de localisation
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_ech.c	1.11 12/23/96 : MSRA18_raf_objets_mobiles_echang";
   char vl_nom_obj[256],
        vl_nom_etat[256],
        vl_nom_transx[256],
        vl_nom_transy[256],
        vl_nom_loc[20],
        vl_nom_var[256],
        vl_cle_mob[256],
        vl_cle[256];
        
   int vl_is_f,vl_is_w;
   int vl_result;
   double vl_x,vl_y;
   int	      vl_ind;
   float      vl_etat;
   float      *pl_etat;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, 
   "IN  : MSRA18_raf_objets_mobiles_echang  va_typeraf:%d; va_no_fmc.numero:%d; pa_nom_ech:%s;va_sens:%d; va_localisat:%d;  ", 
    va_typeraf , va_no_fmc.numero , pa_nom_ech , va_sens , va_localisation  );
        
   /*A Elimination des blancs a la fin du nom de l'echangeur */
   vl_ind = strlen( pa_nom_ech )-1;
   if ( vl_ind > -1 )
      {
      while ( ( vl_ind > 0 ) && ( pa_nom_ech[vl_ind]==' ' ) )
        vl_ind--;
   
      if ( pa_nom_ech[0] ==' ' )
         pa_nom_ech[0] = '\000';
      else
         pa_nom_ech[vl_ind+1] = '\000';
      }
    else
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Pas de nom echangeur  %s" , pa_nom_ech );
      return XDC_NOK;
      }
   
      
   /*A Determination de la localisation */
   switch ( va_localisation )
      {
      case CM_LOC_A:
         strcpy( vl_nom_loc , "a" );
         break;
      case CM_LOC_B:
         strcpy( vl_nom_loc , "b" );
         break;
      case CM_LOC_C:
         strcpy( vl_nom_loc , "c" );
         break;
      case CM_LOC_AB:
         strcpy( vl_nom_loc , "ab" );
         break;
      case CM_LOC_BC:
         strcpy( vl_nom_loc , "bc" );
         break;
      case CM_LOC_AC:
         strcpy( vl_nom_loc , "ac" );
         break;
      default:
         strcpy( vl_nom_loc , "a" );
         break;
      }
         
   sprintf( vl_cle , "!%d!%d!%d!%d!%s!" , va_no_fmc.numero , va_no_fmc.cle , va_no_act.numero , va_no_act.cle , pa_nom_ech);
   sprintf( vl_nom_obj , "Mo%03de%s" , va_type_mobile , vl_cle );
   sprintf( vl_nom_transx , "Mo%03de%s_translationx" , va_type_mobile , vl_cle );
   sprintf( vl_nom_transy , "Mo%03de%s_translationy" , va_type_mobile , vl_cle );
   
   sprintf( vl_nom_etat , "echangeur_%s_etat" , pa_nom_ech  );

   /*B Recherche de la variable locech_xxxx et recuperation de la liste des vues ou elle apparait */
   sprintf( vl_nom_var , "locech_ %s %s_%d" , pa_nom_ech , vl_nom_loc , va_sens );
      
   /*A Creation de la cle pour la table des mobile */ /*
   sprintf( vl_cle_mob , "%s_%03de#%s", vl_cle, va_type_mobile , pa_nom_ech );
   if ( strlen( vl_cle_mob ) > CM_LONG_CLEF )
      {
      vl_cle_mob[CM_LONG_CLEF]='\000';
      XZST_03EcritureTrace(   XZSTC_WARNING, " Clef aff voie trop longue: tronquee : %s" , vl_cle_mob );
      }
      
   strcpy( vl_mob.clef , vl_cle_mob );
      */

   //Construction du message pour le synoptique FLASH
   sprintf( vl_msg , "|8|%d|%d|%d|%d|%s|%d|%d|%d|%d|%s" , va_no_fmc.numero , va_no_fmc.cle , va_no_act.numero , va_no_act.cle , pa_nom_ech, va_typeraf, va_type_mobile, (int) va_sens, (int) va_localisation, vl_nom_obj);
   if (canWeSend(vl_msg)) {
   if (send_message(vl_msg, CM_BUFF_SOCK,"") == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer l'objet mobile sur l'�changeur");
      return XDC_NOK;
      }
        
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA18_raf_objets_mobiles_echang" );
   return XDC_OK;
  }
}



