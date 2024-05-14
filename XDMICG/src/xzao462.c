/*E*/
/* Fichier : $Id: xzao462.c,v 1.4 2018/02/09 15:21:29 devgfi Exp $        Release : $Revision: 1.4 $        Date : $Date: 2018/02/09 15:21:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao462.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       14 Nov 2011   1.1  : Creation (DEM/1014)
* VR       27 Jui 2012   1.2  : Lecture Nom de zones au lieu des Id
* JPL	13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.3
* JPL	07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao462.c,v 1.4 2018/02/09 15:21:29 devgfi Exp $ : xzao462" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des destination temps de parcours et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao462_Lire_Config_Destination_TDP_PMV ( 	FILE  		*pa_Fd,
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
*       xzao462_Lire_Config_Destination_TDP 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao462.c,v 1.4 2018/02/09 15:21:29 devgfi Exp $ : xzao462_Lire_Config_Destination_TDP_PMV" ;
   
   XZAOT_ConfDestPMV	vl_Dest;
   char			vl_Lig[900];
   char			*vl_Texte;
   char			vl_Texte_out[900];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
	int		vl_init=1;

   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao462: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant � pa_Fd
   */
    
      	vl_init = 1; 
/*   while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )*/
   while (fgets(vl_Lig,1000,pa_Fd) != NULL)

   {
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : chaine <%s> taille <%d>",
										   vl_Lig,
										   sizeof(vl_Lig));
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
             ** Extraction des differents champs de vl_Dest
             */
	
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : Saisie Autoroute non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy( vl_Dest.Autoroute, vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR ); 
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : Saisie PR non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Dest.PR = atoi( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : Saisie sens non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Dest.Sens = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR ); 

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {
            	 XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : Nom de destination");
            	 XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : retourne %d",XDC_NOK);
            	 return ( XDC_NOK );
             }
             strcpy( vl_Dest.NomDest, vl_Texte_out );
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );


             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : Saisie type destination non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Dest.TypeDest = atoi ( vl_Texte_out );
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

                         if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
                         {
                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : Saisie sens destination non valide");
                            XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : retourne %d",XDC_NOK);
                            return ( XDC_NOK );
                         }

            vl_Dest.SensDest = atoi ( vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : Saisie Inhibition (%s) non valide",vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Dest.Inhibition = atoi ( vl_Texte_out );

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil == NULL) 
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao462: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }

				/*decodage de la serie des zdp entre le PMV et la destination*/
				while ( (vl_Texte=strtok(NULL,XZAO_TAB_SEPAR)) != NULL)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 zdp : vl_Texte <%s>",
																vl_Texte);

					if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,
							XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
					{
						 XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : Saisie desti(%s) non valide",vl_Texte);
						 XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : retourne %d",XDC_NOK);
						 return ( XDC_NOK );
					}

					strcpy( vl_Dest.Nom_ZDP, vl_Texte_out );
					vl_Dest.Init=vl_init;

						/*B
						** Appel de la fonction utilisateur passee en parametre
						*/
					XZST_03EcritureTrace( XZSTC_WARNING,"IN : xzao462_Lire_Config_Destination_TDP_PMV : Autoroute = %s\tPR = %d\tSens = %d\tDestination: %s\tType: %d\tSensDest = %d\tNom ZDP=%s\tinhibition:%d\tinit:%d\n",
											vl_Dest.Autoroute,
											vl_Dest.PR,
											vl_Dest.Sens,
											vl_Dest.NomDest,
											vl_Dest.TypeDest,
											vl_Dest.SensDest,
											vl_Dest.Nom_ZDP,
											vl_Dest.Inhibition,
											vl_Dest.Init);

					if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_Dest, vl_num_ligne ) ) != XDC_OK )
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 : appel de FonctionUtilisateur s'est mal deroule");
						return ( XDC_NOK );
					}
					vl_init = 0;
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
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
   */
   
*pa_Resultat = 0;

return XDC_OK;

}   

/* Fin de la fonction  */

