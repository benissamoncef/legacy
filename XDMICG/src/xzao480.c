/*E*/
/*Fichier : $Id: xzao480.c,v 1.1 2016/10/13 14:56:33 pc2dpdy Exp $      Release : $Revision: 1.1 $      Date : $Date: 2016/10/13 14:56:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao480.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao480.h"
#include "xzao730.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao480.c,v 1.1 2016/10/13 14:56:33 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2016/10/13 14:56:33 $" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des PMVA  et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao480_Lire_Config_Eqt_SAGA ( 	FILE  		*pa_Fd,
					XDY_FonctionInt pa_FonctionUtil,
					XDY_Entier   	*pa_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* FILE			pa_Fd
* XDY_FonctionInt	pa_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*pa_Resultat:OK=NULL ou ligne de l'erreur du systeme
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
*       XZAOT_ConfEqtSAGA 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "@(#)xzao480.c	1.2 12/15/94 : xzao480_Lire_Config_PMV" ;
   
   XZAOT_ConfEqtSAGA	vl_PMVA;
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   long 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao480: fichier non existant");
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

	  XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s", vl_Lig);
          if (vl_retcode != XDC_OK) 

          {
             /*A
             ** Extraction des differents champs de vl_District
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie XDC_Lon_NomEqt non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.NomEqtSAGA, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie XDC_Lon_NomAuto non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.Autoroute, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie XZAO_LONG_LONG non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PMVA.PR = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie XZAO_LONG_UCHAR non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PMVA.Sens = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PMVA.TypeSAGA = atoi ( vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 30, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.Instance, vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 3, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.FelsCode, vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 3, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.EqtCode, vl_Texte_out );
              

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 3, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.CodeCmdMessage, vl_Texte_out );
              

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 3, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.CodeCmdLuminosite, vl_Texte_out );
              

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 3, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.CodeCmdPicto, vl_Texte_out );
             

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 3, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.CodeCmdHeure, vl_Texte_out );
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomSite, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.NomSite, vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : %s",vl_Texte);  
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 50, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMVA.Alarme, vl_Texte_out );
             
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : felscode %s", vl_PMVA.FelsCode);
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : eqtcode %s", vl_PMVA.EqtCode);
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : alarme %s", vl_PMVA.Alarme);
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao480 : NomSite %s", vl_PMVA.NomSite);
             
             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_PMVA, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao480: appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao480: echec de l'appel de la fonction utilisateur");
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
          *pa_Resultat = NULL;
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier pa_Fd 
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
   */
   
*pa_Resultat = NULL;

return XDC_OK;

}   

/* Fin de la fonction xzao480_Lire_Config_PMVA */

