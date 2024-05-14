/*E*/
/*Fichier : $Id: xzao649.c,v 1.1 2016/12/08 16:18:55 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2016/12/08 16:18:55 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao649.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL		14 Nov 2016	: Creation (LOT23)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao649.c,v 1.1 2016/12/08 16:18:55 pc2dpdy Exp $ : xzao649" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des viaducs et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int xzao649_Lire_Config_Viaduc ( 	FILE  		*va_Fd,
				        XDY_FonctionInt va_FonctionUtil,
				        XDY_Entier   	*va_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fichier		va_Fd
* XDY_Fonction		va_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*va_Resultat:OK=NULL ou ligne de l'erreur du systeme
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
*       XZAOT_ConfViaduc	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao649.c,v 1.1 2016/12/08 16:18:55 pc2dpdy Exp $ : xzao649_Lire_Config_Viaduc" ;
   
   XZAOT_ConfViaduc	vl_Viaduc;
   char			vl_Lig[285];
   char			vl_String[285];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   int			vl_NbCarLu = 0;
   int			vl_Longueur = 0;
   XDY_Entier		vl_Resultat_out = 0;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (va_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao649: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant È va_Fd
   */
    
   while ( ( vl_retcode = XZAO000_LireLigneFich ( va_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++; 
       
       if (vl_Resultat_out != XZAO_EOF )
       { 
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */
          
          *va_Resultat = vl_num_ligne;
             
          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presence du caractere #
          */
          
          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != 0) 

          {
             /*A
             ** Extraction des differents champs 
             */
    
            	XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : %s ",vl_Lig);
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : %s ",vl_Texte_out); 
             strcpy ( vl_Viaduc.NomViaduc, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : %s ",vl_Texte_out); 

             strcpy ( vl_Viaduc.Autoroute, vl_Texte_out );
              
             
             
             /*vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Viaduc.Sens = atoi ( vl_Texte_out );*/
	     vl_Viaduc.Sens = XDC_SENS_SUD;
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : %s ",vl_Texte_out); 
             
             sscanf ( vl_Texte_out, "%d", &vl_Viaduc.PRdebut );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : %s ",vl_Texte_out); 
             sscanf ( vl_Texte_out, "%d", &vl_Viaduc.PRfin );
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
	     
	     if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomSite, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao649 : %s ",vl_Texte_out); 
            strcpy( vl_Viaduc.NomSite, vl_Texte_out ); 
             
             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (va_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *va_FonctionUtil ) ( vl_Viaduc, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao649: appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao649: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier va_Fd 
       ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
       */
       
       {  
          *va_Resultat = NULL;
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier va_Fd 
   ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
   */
   
*va_Resultat = NULL;

return XDC_OK;

}   
/* Fin de la fonction xzao649_Lire_Config_Viaduc */

