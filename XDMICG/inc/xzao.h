/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER XZAO.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Aug 1994	: Creation
* mercier	11 Oct 1994	: ajout de xzat.h
* volcic        11 Oct 1994     : ajout de .h (v1.2)
* volcic        11 Oct 1994     : ajout de .h (v1.3)
* volcic        13 Oct 1994     : ajout de .h (v1.4)
* volcic	21 Oct 1994	: change XZAO_LINE_FEED (v1.5)
* niepceron     28 Jun 1995	: Ajout xzao123.h 1.10 
* Torregrossa   28 Nov 1995	: Ajout xzao316.h 1.12 
* Torregrossa   01 Dec 1995	: Ajout xzao190.h 1.13 
* Mismer	06 Nov 1996	: Ajout xzao187 188 189 196 197 198 1.14  (DEM/1232)
* niepceron     16 Jan 1997	: Ajout de xzao204 (dem/1373) 1.15
* D.Mismer      10 Fev 1997     : Ajout xzao400 401 402 403 404 405 406 407 408 409 (DEM/1395) (v1.16)
* D.Mismer      27 Fev 1997     : Ajout xzao410 (DEM/1395) (v1.17)
* Guilhou	29 sep 1997	: ajout xzao412 (dem/1502) 1.18
* Guilhou	10 dec 1997	: ajout xzao413 et xzao22 (dem/1534) 1.19
* D.Mismer      10 Fev 1997     : Ajout xzao400 401 402 403 404 405 406 407 408 409 (DEM/1395) (v1.16)
* D.Mismer      27 Fev 1997     : Ajout xzao410 (DEM/1395) (v1.17)
* Guilhou	29 sep 1997	: ajout xzao412 (dem/1502) 1.18
* Torregrossa   22 Jan 1998	: Ajout xzao418.h, xzao421.h, xzao422.h, xzao420.h (1545) 1.19 
* JPL		28/06/2007	: Ajout xzao250.h a posteriori; ajout xzao225.h (DEM 657) 1.21
* AAZ       25/09/2007  : Ajout xzao630->xzao636 DEM 662
* JPL		07/10/2010	: Ajout xzao192.h et xzao193.h (DEM 948)  1.23
* JPL		30/11/10	: Migration architecture HP ia64 (DEM 961) : fichiers en-tete systeme inclus  1.24
* JPL		14/01/11	: Migration HP ia64 (DEM 971) : corr. xzao419; condition d'inclusion unique  1.25
* LCL		30/01/12	: TdPetendus (DEM 1014) 1.26
* VR		15/03/12	: ajout PRV (DEM/1016) 1.27
* JPL		25/07/12	: Suppression xzao312, acces table numeros echangeurs (DEM 1033)  1.28
* VR		05/06/12	: Ajout DTP PMVA (DEM/1014 PMA)
* JPL		11/02/13	: Suppression xzao316.h ; ajout xzao25.h, xzao26.h, xzao27.h et xzao28.h  1.30
* JMG		16/04/16 	: ajout xzao790 et xzao910 1.31
* LCL		15/11/16	: ajout xzao649 xzao650 xzao651 1.32 (LOT23) + DEM1191
* LCL		26/06/17	: ajout xzao952 xzao953 1.35 (convergence)
* JPL		06/10/17	: Ajout de xzao4001.h (DEM 1236)  1.36
* JPL		20/10/17	: Remplacement de xzao400.h par xzao4001.h (DEM 1236)  1.37
* JPL		10/01/18	: Ajout xzao261.h et xzao262.h (DEM 1266)  1.38
* LCL		21/02/18	: Ajout xzao258.h xzao259.h xzao260.h xzao860.h CTRL_FEUX 1.39 DEM1284
* LCL		11/05/18	: Ajout GABEB xzao263/264/265/861 1.40 DEM1283
* CGR		27/07/20	: Ajout IMU xzao491/492/493 1.41 DEM-SAE155
* GR            05/08/20        : Ajout xzao960.h xzao962.h TDP_FCD 1.41 DEM-SAE152 
* CGR		02/10/20	: Ajout xzao495/496/498/499 Zones et Messages IMU 1.42 DEM-SAE155
* ABE		15/12/20	: Ajout xzao722 pour Liste_IMU_Machine 1.43 DEM-SAE155 + XZAO9999 MOVIS PRA DEM-SAE93
* ABE       02/08/22    : Ajout xzao507 pour NIC et IS localisation tunnel DEM-SAE403
* ABE       17/10/22    : Ajout fonction xzao29_calcul_zone_hors_reseau DEM-SAE318
* LCL		04/10/23	: Adaptation CNA DEM-483
* JPL		23/01/24	: Ajout xzao963.h (SAE-504)
* GGY		16/02/24	: Ajout XZAO510_Ajouter_Batiment (DEM6483)
------------------------------------------------------ */

#ifndef _XZAO_COMPLET_
#define _XZAO_COMPLET_

/* fichiers inclus */


#include <fcntl.h>

#if defined (__hp9000s300)
#include <tcio.h>
#endif

#include "xdy.h"
#include "asql_com.h"
#include "xzat.h"
#include "xzao000.h"
#include "xzao00.h"
#include "xzao20.h"
#include "xzao21.h"
#include "xzao22.h"
/*
#include "xzao25.h"
#include "xzao26.h"
#include "xzao27.h"
#include "xzao28.h"
*/
#include "xzao29.h"
#include "xzao50.h"
#include "xzao51.h"
#include "xzao52.h"
#include "xzao53.h"
#include "xzao54.h"
#include "xzao55.h"
#include "xzao56.h"
#include "xzao57.h"
#include "xzao58.h"
#include "xzao59.h"
#include "xzao61.h"
#include "xzao62.h"
#include "xzao63.h"
#include "xzao64.h"
#include "xzao65.h"
#include "xzao66.h"
#include "xzao67.h"
#include "xzao68.h"
#include "xzao69.h"
#include "xzao70.h"
#include "xzao71.h"
#include "xzao72.h"
#include "xzao73.h"
#include "xzao74.h"
#include "xzao75.h"
#include "xzao76.h"
#include "xzao77.h"
#include "xzao78.h"
#include "xzao79.h"
#include "xzao80.h"
#include "xzao81.h"
#include "xzao82.h"
#include "xzao84.h"
#include "xzao85.h"
#include "xzao86.h"
#include "xzao87.h"
#include "xzao88.h"
#include "xzao89.h"
#include "xzao90.h"
#include "xzao91.h"
#include "xzao92.h"
#include "xzao93.h"
#include "xzao94.h"
#include "xzao95.h"
#include "xzao96.h"
#include "xzao97.h"
#include "xzao98.h"
#include "xzao99.h"
#include "xzao100.h"
#include "xzao101.h"
#include "xzao102.h"
#include "xzao103.h"
#include "xzao104.h"
#include "xzao105.h"
#include "xzao106.h"
#include "xzao107.h"
#include "xzao108.h"
#include "xzao109.h"
#include "xzao110.h"
#include "xzao111.h"
#include "xzao112.h"
#include "xzao113.h"
#include "xzao114.h"
#include "xzao115.h"
#include "xzao116.h"
#include "xzao117.h"
#include "xzao118.h"
#include "xzao119.h"
#include "xzao121.h"
#include "xzao122.h"
#include "xzao123.h"
#include "xzao124.h"
#include "xzao125.h"
#include "xzao126.h"
#include "xzao127.h"
#include "xzao128.h"
#include "xzao129.h"
#include "xzao130.h"
#include "xzao131.h"
#include "xzao132.h"
#include "xzao133.h"
#include "xzao134.h"
#include "xzao135.h"
#include "xzao136.h"
#include "xzao137.h"
#include "xzao138.h"
#include "xzao139.h"
#include "xzao140.h"
#include "xzao141.h"
#include "xzao142.h"
#include "xzao143.h"
#include "xzao144.h"
#include "xzao146.h"
#include "xzao147.h"
#include "xzao148.h"
#include "xzao149.h"
#include "xzao151.h"
#include "xzao152.h"
#include "xzao154.h"
#include "xzao155.h"
#include "xzao156.h"
#include "xzao157.h"
#include "xzao158.h"
#include "xzao159.h"
#include "xzao160.h"
#include "xzao161.h"
#include "xzao163.h"
#include "xzao164.h"
#include "xzao165.h"
#include "xzao166.h"
#include "xzao0166.h"
#include "xzao167.h"
#include "xzao168.h"
#include "xzao169.h"
#include "xzao170.h"
#include "xzao172.h"
#include "xzao173.h"
#include "xzao174.h"
#include "xzao175.h"
#include "xzao176.h"
#include "xzao177.h"
#include "xzao178.h"
#include "xzao179.h"
#include "xzao180.h"
#include "xzao181.h"
#include "xzao182.h"
#include "xzao183.h"
#include "xzao184.h"
#include "xzao187.h"
#include "xzao188.h"
#include "xzao189.h"
#include "xzao190.h"
#include "xzao191.h"
#include "xzao192.h"
#include "xzao193.h"
#include "xzao196.h"
#include "xzao197.h"
#include "xzao198.h"
#include "xzao203.h"
#include "xzao2031.h"
#include "xzao204.h"
#include "xzao205.h"
#include "xzao208.h"
#include "xzao214.h"
#include "xzao225.h"
#include "xzao250.h"
#include "xzao258.h"
#include "xzao259.h"
#include "xzao260.h"
#include "xzao261.h"
#include "xzao262.h"
#include "xzao263.h"
#include "xzao264.h"
#include "xzao265.h"
#include "xzao301.h"
#include "xzao306.h"
#include "xzao312.h"
#include "xzao317.h"
#include "xzao4001.h"
#include "xzao401.h"
#include "xzao470.h"
#include "xzao402.h"
#include "xzao403.h"
#include "xzao404.h"
#include "xzao405.h"
#include "xzao406.h"
#include "xzao407.h"
#include "xzao408.h"
#include "xzao409.h"
#include "xzao410.h"
#include "xzao412.h"
#include "xzao413.h"
#include "xzao416.h"
#include "xzao417.h"
#include "xzao418.h"
#include "xzao419.h"
#include "xzao420.h"
#include "xzao421.h"
#include "xzao422.h"
#include "xzao461.h"
#include "xzao462.h"
#include "xzao463.h"
#include "xzao465.h"
#include "xzao466.h"
#include "xzao467.h"
#include "xzao468.h"
#include "xzao469.h"
#include "xzao491.h"
#include "xzao492.h"
#include "xzao493.h"
#include "xzao495.h"
#include "xzao496.h"
#include "xzao498.h"
#include "xzao499.h"
#include "xzao506.h"
#include "xzao507.h"
#include "xzao510.h"
#include "xzao511.h"
#include "xzao553.h"
#include "xzao554.h"
#include "xzao556.h"
#include "xzao557.h"
#include "xzao558.h"
#include "xzao563.h"
#include "xzao576.h"
#include "xzao5761.h"
#include "xzao577.h"
#include "xzao5771.h"
#include "xzao570.h"
#include "xzao578.h"
#include "xzao600.h"
#include "xzao601.h"
#include "xzao602.h"
#include "xzao603.h"
#include "xzao604.h"
#include "xzao605.h"
#include "xzao606.h"
#include "xzao607.h"
#include "xzao630.h"
#include "xzao631.h"
#include "xzao632.h"
#include "xzao633.h"
#include "xzao634.h"
#include "xzao635.h"
#include "xzao636.h"
#include "xzao637.h"
#include "xzao649.h"
#include "xzao650.h"
#include "xzao651.h"
#include "xzao722.h"
#include "xzao790.h"
#include "xzao807.h"
#include "xzao816.h"
#include "xzao819.h"
#include "xzao824.h"
#include "xzao825.h"
#include "xzao826.h"
#include "xzao860.h"
#include "xzao861.h"
#include "xzao891.h"
#include "xzao892.h"
#include "xzao910.h"
#include "xzao911.h"
#include "xzao952.h"
#include "xzao953.h"
#include "xzao960.h"
#include "xzao962.h"
#include "xzao963.h"
#include "xzao977.h"
#include "xzao978.h"
#include "xzao979.h"
#include "xzao980.h"
#include "xzao981.h"
#include "xzao982.h"
#include "xzao983.h"
#include "xzao984.h"
#include "xzao985.h"
#include "xzao986.h"
#include "xzao987.h"
#include "xzao988.h"
#include "xzao989.h"
#include "xzao990.h"
#include "xzao991.h"
#include "xzao992.h"
#include "xzao993.h"
#include "xzao994.h"
#include "xzao995.h"
#include "xzao996.h"
#include "xzao997.h"
#include "xzao998.h"
#include "xzao999.h"
#include "xzao1000.h"
#include "xzao1001.h"
#include "xzao1002.h"
#include "xzao1003.h"
#include "xzao1004.h"
#include "xzao1005.h"
#include "xzao1006.h"
#include "xzao1007.h"
#include "xzao1008.h"
#include "xzao1009.h"
#include "xzao1010.h"
#include "xzao1011.h"
#include "xzao1012.h"
#include "xzao1013.h"
#include "xzao1014.h"
#include "xzao9999.h"

#include "xzav01.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
