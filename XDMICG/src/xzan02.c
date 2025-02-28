/*E*/
/*Fichier : $Id: xzan02.c,v 1.4 2018/02/09 15:17:38 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/02/09 15:17:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan02.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       26 Oct 1994     : Creation
* JPL	05/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.3
* JPL	07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzan.h"

static char *version = "$Id: xzan02.c,v 1.4 2018/02/09 15:17:38 devgfi Exp $: xzan02.c" ;
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit le fichier de configuration des types d'astreintes et
*    appelle la fonction utilisateur de GECONF
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAN02_Lire_Type_Astreinte (	FILE  		*pa_Fd,
				        XDY_FonctionInt pa_FonctionUtil,
				        XDY_Entier   	*pa_Resultat )

/*
*
* PARAMETRES EN ENTREE :
*
* FILE			pa_Fd	
* XDY_FonctionInt 	pa_FonctionUtil	
*
* PARAMETRES EN SORTIE :	
*
* XDY_Entier   		pa_Resultat
*
* VALEUR RENDUE :
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*    Appelle la fonction utilisateur avec les arguments:
*       XDY_Nom 		NomTypeAstreinte
*	XDY_Octet		NumAstreinte
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{

static char *version = "$Id: xzan02.c,v 1.4 2018/02/09 15:17:38 devgfi Exp $ : XZAN02_Lire_Type_Astreinte" ;
   
   XZAOT_ConfPtMes	vl_PtMes;
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
   XDY_Nom		vl_NomAstreinte;
   XDY_Octet		vl_NumAstreinte = 0;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzan02: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant � pa_Fd
   */
    
   while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier pa_Fd 
       */
    
       vl_num_ligne++; 
       
       if (vl_Resultat_out != XZAO_EOF )
       { 
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */
          
          *pa_Resultat = vl_num_ligne;
             
          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presence du caractere #
          */
          
          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != XDC_OK) 

          {
             /*A
             ** Extraction des differents champs de vl_District
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzan02 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzan02 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_NomAstreinte, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzan02 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzan02 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_NumAstreinte = atoi ( vl_Texte_out ); 
             
             
              
             
             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_NomAstreinte, vl_NumAstreinte, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzan02 : appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzan02: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier pa_Fd 
       ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
       */
       
       {  
          *pa_Resultat = 0;
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier pa_Fd 
   */


   /*A
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
   */
   
*pa_Resultat = 0;

return ( XDC_OK );

}




