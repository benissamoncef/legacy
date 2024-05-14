/*E*/
/*Fichier : $Id: xzao423sp.prc,v 1.16 2017/07/04 16:32:45 gesconf Exp $     Release : $Revision: 1.16 $        Date :  $Date: 2017/07/04 16:32:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao423sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* guilhou       11/10/00        : creation 1.1
* guilhou	12/12/00	: verrue pour evt qui contient bifurc A520 1.2
* cluchague     26/12/01        : refonte complete "en dur" 1.3
* cluchague     04/06/02        : 1.4 dans le cas repere 1 on doit quand meme avoir 
*                                 @va_presence_bifurcation=1
* cluchague     18/06/02        : 1.5 on autorise le chemin A8/2 -> A50 / 2 (par A57) REPERE2
* cluchague     05/08/03        : 1.6 on rajoute une bifurc sur A8 pour A500 REPERE3
* cluchague     07/08/03        : 1.7 attention la bifurc n'est qu'en sens 1 
* JMG+CLU	31/01/06	: 1.8 variable va_distance_out initialisee a 99999 par defaut au debut + bug A501
* cluchague     20/09/06        : 1.9 degug A52/2 -> A520/1 (ligne 531 ) pour rapppel 99999 traduit les trajets impossibles
* cluchague     06/02/07        : 1.10 On peut d�sormais calculer de bretelle � bretelle (les sens 3,4,7, 8 pas geres)
* cluchague     13/12/07        : 1.11 sur a8 s1 si d est apres bif monaco direction=italie et non pas nice REPERE11 
* cluchague     03/03/08        : 1.12 la traversee d'aix est pr�cisee : 11268 au lieu de 6000 REPERE12 
* PNI		11/02/13	: 1.13 integration du PMV A501-1900-1
* PNI		29/11/16	: 1.14 integration des PMVA SOPHIA sur D35 et D535 DEM1202 
* JMG   20/03/17       		: 1.15 SAGA bifurcation  DEM1191 
* PNI		04/07/2017	: 1.16 integration des PMVA SOPHIA sur D35 et D535
* ABE   20/10/2022  : Ajout A7 pour prise en compte des bouchons sur reseau ASF DEM-SAE412
* PNI   16/08/2023  : ajout direction LYON pour A8 PR<17900
* PNI   13/11/2023  : correction d'un end mal placé SAE-547
* LCL   05/01/2024  : Ajout config CNA SAE-483
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao423sp.h" */

/* static char *version = " $Id: xzao423sp.prc,v 1.16 2017/07/04 16:32:45 gesconf Exp $: xzao423" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
* calcule la distance entre un point origine et un point destination
* en tenant compte des bifurcations eventuelles
* Rappel : 99999 traduit les trajets impossibles !!!!!!!!!!!!!!!!!!!!!!
* Rappel : 99998 traduit les cas imprevus, non penses on passe par la ligne "1" du programme et on rentre dans aucun des switch/case
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go
drop procedure XZAO423
go

create procedure XZAO423
   @va_autoroute_in_o    tinyint = null,
   @va_PR_in_o           int = null, 
   @va_sens_in_o         tinyint = null, 
   @va_autoroute_in_d    tinyint = null,
   @va_PR_in_d           int = null, 
   @va_sens_in_d         tinyint = null,  
   @va_PR2_in_d          int = null, 
   @va_distance_out      int output,
   @va_presence_bifurcation_out int output,
   @va_destination char(15) output
   as 

   declare @vl_A8     tinyint ,
	   @vl_A500   tinyint ,
	   @vl_A50    tinyint ,
	   @vl_A501   tinyint ,
	   @vl_A51    tinyint ,
	   @vl_A52    tinyint ,
	   @vl_A520   tinyint ,
	   @vl_A57    tinyint ,
	   @vl_A7    tinyint ,
	   @vl_D35    tinyint ,
	   @vl_D535    tinyint ,
     @vl_A11    tinyint,
           @vl_traversee_toulon int ,
	   @vl_traversee_aix    int ,
	   @vl_AUBAGNE char(15) ,
	   @vl_TOULON char(15) ,
	   @vl_TOULON_PAR_CANNET char(15),
	   @vl_CANNET_PAR_TOULON char(15),
	   @vl_CANNET char(15),
	   @vl_VERS   char(15),
	   @vl_MONACO char(15),
	   @vl_MARSEILLE char(15),
	   @vl_LES_ALPES char(15),
	   @vl_NICE char(15),
	   @vl_AIX char(15),
	   @vl_AURIOL char(15),
	   @vl_BIF_A8 char(15),
	   @vl_FRONTIERE char(15),
	   @vl_ITALIE char(15),
	   @vl_LA_TURBIE char(15),
	   @vl_AIX_NICE  char(15),
	   @vl_TOULON_PAR_AUBAGNE char(15),
	   @vl_AUBAGNE_PAR_TOULON char(15),
	   @vl_LYON char(15),
     @vl_NANTES char(15),
     @vl_PARIS char(15)

   select  @vl_A8             = 1,
	   @vl_A500           = 2,
	   @vl_A50            = 3,
	   @vl_A501           = 4,
	   @vl_A51            = 5,
	   @vl_A52            = 6,
	   @vl_A520           = 7,
	   @vl_A57            = 8,
     @vl_A7             = 9,
	   @vl_D35            = 14,
	   @vl_D535            = 15,
     @vl_A11            = 101,
           @vl_traversee_toulon     = 10000,
	   @vl_traversee_aix        = 11268,
	   @vl_AUBAGNE          ="-> AUBAGNE",
	   @vl_TOULON          ="-> TOULON",
	   @vl_TOULON_PAR_CANNET          ="->A57->TOULON",
	   @vl_CANNET_PAR_TOULON          ="->TOULON->A57",
	   @vl_CANNET          ="-> CANNET",
	   @vl_MONACO          ="-> MONACO",
	   @vl_MARSEILLE          ="-> MARSEILLE",
	   @vl_LES_ALPES          ="-> LES ALPES",
	   @vl_NICE          ="->   NICE",
	   @vl_AIX          ="->    AIX",
	   @vl_AURIOL          ="->  AURIOL",
	   @vl_BIF_A8          ="-> AIX / NICE",
	   @vl_FRONTIERE          ="-> FRONTIERE",
	   @vl_ITALIE          ="-> ITALIE",
	   @vl_LA_TURBIE          ="-> LA TURBIE",
	   @vl_AIX_NICE           ="-> AIX / NICE",
	   @vl_TOULON_PAR_AUBAGNE          ="->A50->TOULON",
	   @vl_AUBAGNE_PAR_TOULON          ="->TOULON->A50",
	   @vl_VERS                        ="->   ",
     @vl_LYON                ="->    LYON",
	   @vl_NANTES              ="-> NANTES",
     @vl_PARIS               ="-> PARIS"


if @va_sens_in_o in (XDC_SENS_SORTIE_NORD,XDC_SENS_ENTREE_NORD) select  @va_sens_in_o = XDC_SENS_NORD
if @va_sens_in_o in (XDC_SENS_SORTIE_SUD,XDC_SENS_ENTREE_SUD) select  @va_sens_in_o = XDC_SENS_SUD
if @va_sens_in_d in (XDC_SENS_SORTIE_NORD,XDC_SENS_ENTREE_NORD) select  @va_sens_in_o = XDC_SENS_NORD
if @va_sens_in_d in (XDC_SENS_SORTIE_SUD,XDC_SENS_ENTREE_SUD) select  @va_sens_in_o = XDC_SENS_SUD

select @va_distance_out=99998

/********************************************************************/
/*                     REPERE1                                      */
/*  si les point d'origine et de destination ne sont pas sur les    */
/*  memes autoroutes et si l'evenement est long et est dans une     */
/*  bifurcation  on positionne le point de destination au PR de la  */
/*  jonction    sur l'autoroute adjacente                           */
/********************************************************************/
if ( @va_autoroute_in_d != @va_autoroute_in_o )
begin
  if ( @va_autoroute_in_d = @vl_A8 and 
	  (
	     (@va_PR_in_d>30700 and @va_PR2_in_d<30700) or (@va_PR_in_d<30700 and  @va_PR2_in_d>30700)
	  ) 
       and @va_autoroute_in_o in (@vl_A52,@vl_A50,@vl_A57) 
     )
  begin 
    select @va_autoroute_in_d = @vl_A8                           
    select @va_PR_in_d = 30700
  end
  else if ( @va_autoroute_in_d = @vl_A8 and ((@va_PR_in_d>98800 and @va_PR2_in_d<98800) or (@va_PR_in_d<98800 and  @va_PR2_in_d>98800)) 
       and @va_autoroute_in_o in (@vl_A52,@vl_A50,@vl_A57) )
  begin 
    select @va_autoroute_in_d = @vl_A8                           
    select @va_PR_in_d = 98800
  end
  else if ( @va_autoroute_in_d = @vl_A8 and ((@va_PR_in_d>207400 and @va_PR2_in_d<207400) or (@va_PR_in_d<207400 and  @va_PR2_in_d>207400)) 
       and @va_autoroute_in_o = @vl_A500 )
  begin 
    select @va_autoroute_in_d = @vl_A8                           
    select @va_PR_in_d = 207400
  end
  else if ( @va_autoroute_in_d = @vl_A8 and ((@va_PR_in_d>171040 and @va_PR2_in_d<171040) or (@va_PR_in_d<171040 and  @va_PR2_in_d>171040)) 
       and @va_autoroute_in_o = @vl_D35 )
  begin 
    select @va_autoroute_in_d = @vl_A8                           
    select @va_PR_in_d = 171040
  end
  else if ( @va_autoroute_in_d = @vl_A8 and ((@va_PR_in_d>171040 and @va_PR2_in_d<171040) or (@va_PR_in_d<171040 and  @va_PR2_in_d>171040)) 
       and @va_autoroute_in_o = @vl_D535 )
  begin 
    select @va_autoroute_in_d = @vl_A8                           
    select @va_PR_in_d = 171040
  end
  else if ( @va_autoroute_in_d = @vl_A8 and ((@va_PR_in_d>172254 and @va_PR2_in_d<172254) or (@va_PR_in_d<172254 and  @va_PR2_in_d>172254)) 
       and @va_autoroute_in_o = @vl_D35 )
  begin 
    select @va_autoroute_in_d = @vl_A8                           
    select @va_PR_in_d = 172100
  end
  else if ( @va_autoroute_in_d = @vl_A8 and ((@va_PR_in_d>172444 and @va_PR2_in_d<172444) or (@va_PR_in_d<172444 and  @va_PR2_in_d>172444)) 
       and @va_autoroute_in_o = @vl_D535 )
  begin 
    select @va_autoroute_in_d = @vl_A8                           
    select @va_PR_in_d = 172400
  end
  else if ( @va_autoroute_in_d = @vl_A50 and ((@va_PR_in_d>25847 and @va_PR2_in_d<25847) or (@va_PR_in_d<25847 and  @va_PR2_in_d>25847)) )
  begin 
    select @va_autoroute_in_d = @vl_A50                            
    select @va_PR_in_d = 25847
  end
  else if ( @va_autoroute_in_d = @vl_A7 and @va_autoroute_in_o = @vl_A8) 
  begin 
    select @va_autoroute_in_d = @vl_A7                            
    select @va_PR_in_d = 246245
  end
end
-- select "1",@va_autoroute_in_d,@va_sens_in_d,@va_autoroute_in_o,@va_PR_in_d
/********************************************************************/
/*         A8 / 1 -> A8  / 1                                        */
/********************************************************************/
if ( @va_autoroute_in_o = @vl_A8 and @va_sens_in_o = 1 )
begin
if (@va_autoroute_in_d = @vl_A8  and @va_sens_in_d = 1 )
begin 
  /* REPERE11 */
  if @va_PR_in_d > 207400 select @va_destination=@vl_ITALIE
  else select @va_destination=@vl_NICE
  select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
  if ((@va_PR_in_o < 30700 and @va_PR_in_d > 30700 ) 
       or(@va_PR_in_o < 98800 and @va_PR_in_d > 98800) 
       or (@va_PR_in_o < 207400 and @va_PR_in_d > 207400)
     ) select @va_presence_bifurcation_out = 1
  else select @va_presence_bifurcation_out = 0
end
/********************************************************************/
/*         A8 / 1 -> A52 / 1                                        */
/********************************************************************/
else  
if (@va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 1 )
begin 
  if @va_PR_in_o > 30700 select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (30700-@va_PR_in_o) + (@va_PR_in_d)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AUBAGNE
  end
end

/********************************************************************/
/*         A8 / 1 -> A50 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_o > 30700 or @va_PR_in_d < 25847 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (30700-@va_PR_in_o) + (26077) + (@va_PR_in_d-25847) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON
  end
end

/********************************************************************/
/*         A8 / 1 -> A57 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 1 )
begin 
  if @va_PR_in_o > 98800 select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (98800-@va_PR_in_o) + (52800-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON_PAR_CANNET
  end
end

/********************************************************************/
/*         A8 / 1 -> A57 / 2                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 2 )
begin 
  if @va_PR_in_o > 30700 select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (30700-@va_PR_in_o) + (26077) + (67700-25847) + @vl_traversee_toulon + (@va_PR_in_d-6350) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_CANNET_PAR_TOULON
  end
end

/********************************************************************/
/*         A8 / 1 -> A500 / 1                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin 
  if @va_PR_in_o > 207400 select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (207400-@va_PR_in_o) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MONACO
  end
end

/********************************************************************/
/*         A8 / 1 -> A501 / 2                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A501 and @va_sens_in_d = 2 )
begin 
  if @va_PR_in_o > 30700 select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (30700-@va_PR_in_o) + (20900) + (5375-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MARSEILLE
  end
end
end
else
/********************************************************************/
/*         A11 / 1 -> A11  / 1                                        */
/********************************************************************/
if ( @va_autoroute_in_o = @vl_A11 and @va_sens_in_o = 1 )
begin
  if (@va_autoroute_in_d = @vl_A11  and @va_sens_in_d = 1 )
  begin 
    select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
    select @va_presence_bifurcation_out = 0
    select @va_destination=@vl_NANTES
  end
end

/********************************************************************/
/*         A8 / 2 -> A8  / 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A8 and @va_sens_in_o = 2 )
begin
if (@va_autoroute_in_d = @vl_A8  and @va_sens_in_d = 2 )
begin 
  select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
  select @va_presence_bifurcation_out = 0
  if @va_PR_in_d < 18000
  begin
        select @va_presence_bifurcation_out = 1
        select @va_destination=@vl_LYON
  end
  else
  begin
  select @va_destination=@vl_AIX
  if ((@va_PR_in_o > 30700 and @va_PR_in_d < 30700 )
       or(@va_PR_in_o > 98800 and @va_PR_in_d < 98800 )
     ) select @va_presence_bifurcation_out = 1
  end
end
/********************************************************************/
/*         A8 / 2 -> A52 / 1                                        */
/********************************************************************/
else  
if (@va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 1 )
begin 
  if @va_PR_in_o < 30700 select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-30700) + (@va_PR_in_d)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AUBAGNE
  end
end

/********************************************************************/
/*         A8 / 2 -> A50 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_o < 30700 or @va_PR_in_d < 25847 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-30700) + (26077) + (@va_PR_in_d-25847) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON
  end
end

/********************************************************************/
/*         A8 / 2 -> A50 / 2 REPERE2                                */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 2 )
begin 
  if ( @va_PR_in_o < 98800 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-98800) + (52800-6350) + @vl_traversee_toulon + (67700 - @va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AUBAGNE_PAR_TOULON
  end
end

/********************************************************************/
/*         A8 / 2 -> A51 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A51 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (@va_PR_in_o-17900) + @vl_traversee_aix + (@va_PR_in_d-23310) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_LES_ALPES
end

/********************************************************************/
/*         A8 / 2 -> A57 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_o < 98800 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-98800) + (52800-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON
  end
end

/********************************************************************/
/*         A8 / 2 -> A500 / 1                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_o < 207400 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-207400) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MONACO
  end
end

/********************************************************************/
/*         A8 / 2 -> A501 / 2                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A501 and @va_sens_in_d = 2 )
begin
  if ( @va_PR_in_o < 30700 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-30700) + (20900) + (5375-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MARSEILLE
  end
end

/********************************************************************/
/*         A8 / 2  -> A7 / 1                                       */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A7 and @va_sens_in_d = 1 )
begin
  if  @va_PR_in_d > 258510 select @va_distance_out =  99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o) + (@va_PR_in_d-246245) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MARSEILLE
  end 
end

/********************************************************************/
/*         A8 / 2  -> A7 / 2                                       */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A7 and @va_sens_in_d = 2 )
begin
  if  @va_PR_in_d < 233980 select @va_distance_out =  99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o) + (246245-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_LYON
  end 
end
end /*SAE-547*/
/********************************************************************/
/*         A11 / 2 -> A11  / 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A11 and @va_sens_in_o = 2 )
begin
  if (@va_autoroute_in_d = @vl_A11  and @va_sens_in_d = 2 )
  begin 
    select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
    select @va_presence_bifurcation_out = 0
    select @va_destination=@vl_PARIS
  end
end

/********************************************************************/
/*         A52/ 1 -> A52 / 1                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A52 and @va_sens_in_o = 1 )
begin
if (@va_autoroute_in_d = @vl_A52  and @va_sens_in_d = 1 )
begin 
  select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
  if ((@va_PR_in_o < 20900 and @va_PR_in_d > 20900 ) )
  begin
       select @va_presence_bifurcation_out = 1
       select @va_destination=@vl_TOULON 
  end
  else select @va_presence_bifurcation_out = 0
end

/********************************************************************/
/*         A52/ 1 -> A50 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_d < 25847 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (26077-@va_PR_in_o) + (@va_PR_in_d-25847) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON
  end
end

/********************************************************************/
/*         A52/ 1 -> A57 / 2                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 2 )
begin 
    select @va_distance_out =  (26077-@va_PR_in_o) + (67700-25847) + @vl_traversee_toulon + (@va_PR_in_d-6350) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_CANNET
end

/********************************************************************/
/*         A52/ 1 -> A500 / 1                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (26077-@va_PR_in_o) + (67700-25847) + @vl_traversee_toulon + (207400-98800) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MONACO
end

/********************************************************************/
/*         A52/ 1 -> A501 / 2                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A501 and @va_sens_in_d = 2 )
begin 
    select @va_distance_out =  (20900-@va_PR_in_o) + (5375-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MARSEILLE
end
end
/********************************************************************/
/*         A52/ 2 -> A52 / 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A52 and @va_sens_in_o = 2 )
begin
if (@va_autoroute_in_d = @vl_A52  and @va_sens_in_d = 2 )
begin 
  select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
  if ((@va_PR_in_o > 17000 and @va_PR_in_d < 17000 ) )
  begin
      select @va_presence_bifurcation_out = 1
      select @va_destination=@vl_AIX_NICE 
  end
  else select @va_presence_bifurcation_out = 0
end

/********************************************************************/
/*         A52/ 2 -> A8  / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_d < 30700 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o) + (@va_PR_in_d-30700)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_NICE
  end
end

/********************************************************************/
/*         A52/ 2 -> A8  / 2                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 2 )
begin 
  if ( @va_PR_in_d > 30700 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o) + (30700-@va_PR_in_d)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AIX
  end
end

/********************************************************************/
/*         A52/ 2 -> A51 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A51 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (@va_PR_in_o) + (30900-17900) + @vl_traversee_aix + (@va_PR_in_d-23310) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_LES_ALPES
end

/********************************************************************/
/*         A52/ 2 -> A57 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (@va_PR_in_o) + (98800-30700) + (52800-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON_PAR_CANNET
end

/********************************************************************/
/*         A52/ 2 -> A500 / 1                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (@va_PR_in_o) + (207400-30700) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON_PAR_CANNET
end

/********************************************************************/
/*         A52/ 2 -> A520 / 1                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A520 and @va_sens_in_d = 1 )
begin 
  If ( @va_PR_in_o < 17000 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-17000) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AURIOL
  end
end
end
/********************************************************************/
/*         A50/ 1 -> A50 / 1                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A50 and @va_sens_in_o = 1 )
begin
if (@va_autoroute_in_d = @vl_A50  and @va_sens_in_d = 1 )
begin 
  select @va_destination=@vl_TOULON 
  select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
  if ( (@va_PR_in_o < 25847 and @va_PR_in_d > 25847) ) 
    select @va_distance_out = @va_distance_out - (25847 - 16698) 
  select @va_presence_bifurcation_out = 0
end

/********************************************************************/
/*         A50/ 1 -> A8  / 1                                        */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_d < 98800 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (67700-@va_PR_in_o) + @vl_traversee_toulon + (52800-6350) + (@va_PR_in_d-98800) 
    if @va_PR_in_o < 17000 select @va_distance_out = @va_distance_out - (25847 - 16698) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_NICE
  end
end

/********************************************************************/
/*         A50/ 1 -> A57 / 2                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 2 )
begin 
    select @va_distance_out =  (67700-@va_PR_in_o) + @vl_traversee_toulon+(@va_PR_in_d-6350) 
    select @va_distance_out =  (72790-@va_PR_in_o) + (@va_PR_in_d-0) 
    if @va_PR_in_o < 17000 select @va_distance_out = @va_distance_out - (25847 - 16698) 
    select @va_presence_bifurcation_out = 0 /*a la place de 1 depuis SAGA*/
    select @va_destination=@vl_CANNET
end

/********************************************************************/
/*         A50/ 1 -> A500 / 1                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (67700-@va_PR_in_o) +  @vl_traversee_toulon + (@va_PR_in_d) 
    if @va_PR_in_o < 17000 select @va_distance_out = @va_distance_out - (25847 - 16698) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MONACO
end
end
/********************************************************************/
/*         A50/ 2 -> A50 / 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A50 and @va_sens_in_o = 2 )
begin
if (@va_autoroute_in_d = @vl_A50  and @va_sens_in_d = 2 )
begin 
  select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
  if ( ( @va_PR_in_o > 25847 and @va_PR_in_d < 25847 ) )
  begin
    select @va_destination=@vl_MARSEILLE 
    select @va_presence_bifurcation_out = 1
    select @va_distance_out =  @va_PR_in_o - @va_PR_in_d - (25847 - 16698)
  end
  else
  begin
    select @va_presence_bifurcation_out = 0
    select @va_distance_out = @va_PR_in_o - @va_PR_in_d
  end
end

/********************************************************************/
/*         A50/ 2 -> A8  / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_o < 25847 or @va_PR_in_d < 30700 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-25847) +(26077) + (@va_PR_in_d-30700)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_NICE
  end
end

/********************************************************************/
/*         A50/ 2 -> A8  / 2                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 2 )
begin 
  if ( @va_PR_in_o < 25847 or @va_PR_in_d > 30700 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-25847) + (26077) + (30700-@va_PR_in_d)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AIX
  end
end

/********************************************************************/
/*         A50/ 2 -> A52 / 2                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 2 )
begin 
  if ( @va_PR_in_o < 25847 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-25847) + (26077-@va_PR_in_d)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_BIF_A8
  end
end

/********************************************************************/
/*         A50/ 2 -> A51 / 1                                        */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A51 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_o < 25847 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-25847) + (30700-17900) + @vl_traversee_aix + (@va_PR_in_d-23310) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_LES_ALPES
  end
end

/********************************************************************/
/*         A50/ 2 -> A500 / 1                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_o < 25847 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-25847) + (207400-30700) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MONACO
  end
end

/********************************************************************/
/*         A50/ 2 -> A520 / 1                                       */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A520 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_o < 25847 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-25847) + (26077-17000) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AURIOL
  end
end
end
/********************************************************************/
/*         A51/ 1 -> A51 / 1                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A51 and @va_sens_in_o = 1 )
begin
if (@va_autoroute_in_d = @vl_A51  and @va_sens_in_d = 1 )
begin 
  select @va_destination=@vl_LES_ALPES 
  select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
  select @va_presence_bifurcation_out = 0
end
end
/********************************************************************/
/*         A51/ 2 -> A51 / 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A51 and @va_sens_in_o = 2 )
begin
if (@va_autoroute_in_d = @vl_A51  and @va_sens_in_d = 2 )
begin 
  select @va_destination=@vl_AIX 
  select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
  select @va_presence_bifurcation_out = 0
end

/********************************************************************/
/*         A51 / 2  -> A8 / 1                                       */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1 )
begin 
  select @va_distance_out =  (@va_PR_in_o-23310) + @vl_traversee_aix + (@va_PR_in_d-17900)  
  select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_NICE
end

/********************************************************************/
/*         A51 / 2  -> A52 / 1                                      */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (@va_PR_in_o-23310) + @vl_traversee_aix + (30700-17900) + (@va_PR_in_d-0) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AUBAGNE
end

/********************************************************************/
/*         A51 / 2  -> A50 / 1                                      */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 1 )
begin 
  if ( @va_PR_in_d < 25847 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o-23310) + @vl_traversee_aix + (30700-17900) + (26077) + (@va_PR_in_d-25847) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON
  end
end

/********************************************************************/
/*         A51 / 2  -> A57 / 1                                      */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (@va_PR_in_o-23310) + @vl_traversee_aix + (98800-17900) + (52800-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON_PAR_CANNET
end

/********************************************************************/
/*         A51 / 2  -> A57 / 2                                      */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 2 )
begin 
    select @va_distance_out =  (@va_PR_in_o-23310) + @vl_traversee_aix + (30700-17900) + (26077) + (67700-25847) + @vl_traversee_toulon + (@va_PR_in_d-6350) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_CANNET_PAR_TOULON
end

/********************************************************************/
/*         A51 / 2  -> A500 / 1                                     */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (@va_PR_in_o-23310) + @vl_traversee_aix + (207400-17900) + (@va_PR_in_d-0) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MONACO
end

/********************************************************************/
/*         A51 / 2  -> A501 / 2                                     */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A501 and @va_sens_in_d = 2 )
begin 
    select @va_distance_out =  (@va_PR_in_o-23310) + @vl_traversee_aix + (30700-17900) + (20900) + (5375-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MARSEILLE
end
end
/********************************************************************/
/*         A57/ 1 -> A57 / 1                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A57 and @va_sens_in_o = 1 )
begin
if (@va_autoroute_in_d = @vl_A57  and @va_sens_in_d = 1 )
begin 
  select @va_destination=@vl_TOULON 
  select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
  select @va_presence_bifurcation_out = 0
end

/********************************************************************/
/*         A57 / 1  -> A52 / 2                                      */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 2 )
begin 
    select @va_distance_out =  (@va_PR_in_o-6350) + @vl_traversee_toulon + (67700-25847) + (26077-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_BIF_A8
end

/********************************************************************/
/*         A57 / 1  -> A50 / 2                                      */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 2 )
begin 
    select @va_distance_out =  (@va_PR_in_o-6350) + @vl_traversee_toulon + (67700-@va_PR_in_d) 
    select @va_distance_out =  (@va_PR_in_o-0) + (72790-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 0
    select @va_destination=@vl_AUBAGNE
end

/********************************************************************/
/*         A57 / 1  -> A520 / 1                                     */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A520 and @va_sens_in_d = 1 )
begin 
    select @va_distance_out =  (@va_PR_in_o-6350) + @vl_traversee_toulon + (67700-25847) + (26077-17000) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AURIOL
end
end
/********************************************************************/
/*         A57/ 2 -> A57 / 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A57 and @va_sens_in_o = 2 )
begin
if (@va_autoroute_in_d = @vl_A57  and @va_sens_in_d = 2 )
begin 
  select @va_destination=@vl_CANNET 
  select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
  select @va_presence_bifurcation_out = 0
end

/********************************************************************/
/*         A57 / 2  -> A8 / 1                                       */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1 )
begin
  if  @va_PR_in_d < 98800 select @va_distance_out =  99999
  else
  begin
    select @va_distance_out =  (52800-@va_PR_in_o) + (@va_PR_in_d-98800) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_NICE
  end 
end

/********************************************************************/
/*         A57 / 2  -> A8 / 2                                       */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 2 )
begin
  if  @va_PR_in_d > 98800 select @va_distance_out =  99999
  else
  begin
    select @va_distance_out =  (52800-@va_PR_in_o) + (98800-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AIX
  end 
end

/********************************************************************/
/*         A57 / 2  -> A52 / 1                                      */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (52800-@va_PR_in_o) + (98800-30700) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AUBAGNE
end

/********************************************************************/
/*         A57 / 2  -> A51 / 1                                      */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A51 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (52800-@va_PR_in_o) + (98800-17900) + @vl_traversee_aix + (@va_PR_in_d-23310) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_LES_ALPES
end

/********************************************************************/
/*         A57 / 2  -> A500 / 1                                     */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (52800-@va_PR_in_o) + (207400-98800) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MONACO
end
end


/********************************************************************/
/*         A500/1 -> A500/ 1                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A500 and @va_sens_in_o = 1 )
begin
if (@va_autoroute_in_d = @vl_A500  and @va_sens_in_d = 1 )
begin 
  select @va_destination=@vl_MONACO 
  select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
  select @va_presence_bifurcation_out = 0
end
end
/********************************************************************/
/*         A500/2 -> A500/ 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A500 and @va_sens_in_o = 2 )
begin
if (@va_autoroute_in_d = @vl_A500  and @va_sens_in_d = 2 )
begin 
  select @va_destination=@vl_LA_TURBIE 
  select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
  select @va_presence_bifurcation_out = 0
end

/********************************************************************/
/*         A500 / 2  -> A8 / 1                                      */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1 )
begin
  if @va_PR_in_d < 207400 select @va_distance_out =  99999
  begin
    select @va_distance_out =  (@va_PR_in_o) + (@va_PR_in_d-207400) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_FRONTIERE
  end
end

/********************************************************************/
/*         A500 / 2  -> A8 / 2                                      */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 2 )
begin
  if @va_PR_in_d > 207400 select @va_distance_out =  99999
  begin
    select @va_distance_out =  (@va_PR_in_o) + (207400-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AIX
  end
end

/********************************************************************/
/*         A500 / 2  -> A52 / 1                                     */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (@va_PR_in_o) + (207400-30700) + @va_PR_in_d 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AUBAGNE
end

/********************************************************************/
/*         A500 / 2  -> A50 / 1                                     */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 1 )
begin
  if @va_PR_in_d < 25847 select @va_distance_out =  99999
  else
  begin
    select @va_distance_out =  (@va_PR_in_o) + (207400-30700) + (26077) + (@va_PR_in_d-25847) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON_PAR_AUBAGNE
  end
end

/********************************************************************/
/*         A500 / 2  -> A51 / 1                                     */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A51 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (@va_PR_in_o) + (207400-17900) + @vl_traversee_aix + (@va_PR_in_d-23310) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_LES_ALPES
end

/********************************************************************/
/*         A500 / 2  -> A57 / 1                                     */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (@va_PR_in_o) + (207400-98800) + (52800-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_TOULON_PAR_CANNET
end
end


/********************************************************************/
/*         D35/ 2 -> A8  / 2       171040                                 */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 2 and @va_autoroute_in_o = @vl_D35)
begin 
  if ( @va_PR_in_d > 171400 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (5800-@va_PR_in_o) + (171040-@va_PR_in_d)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AIX
  end
end




/********************************************************************/
/*         D35/ 2 -> A8  / 1        172100                               */
/********************************************************************/
else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1  and @va_autoroute_in_o = @vl_D35)
begin 
  if ( @va_PR_in_d < 172100 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (5400-@va_PR_in_o) + (@va_PR_in_d-172100)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_NICE
  end
	
end
/********************************************************************/
/*         D535/ 2 -> A8  / 2                                        */
/********************************************************************/

else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 2 and @va_autoroute_in_o = @vl_D535)
begin 
select @va_autoroute_in_d,@va_sens_in_d,@va_autoroute_in_o,@va_PR_in_d
  if ( @va_PR_in_d > 171400 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (3400-@va_PR_in_o) + (171040-@va_PR_in_d)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AIX
  end
end
/********************************************************************/
/*         D535/ 2 -> A8  / 1                                        */
/********************************************************************/

else 
if (@va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1 and @va_autoroute_in_o = @vl_D535)
begin 
-- select @va_autoroute_in_d,@va_sens_in_d,@va_autoroute_in_o,@va_PR_in_d
  if ( @va_PR_in_d < 172444 ) select @va_distance_out = 99999
  else
  begin
    select @va_distance_out =  (2200-@va_PR_in_o) + (@va_PR_in_d-172444)  
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_NICE
  end
end

/********************************************************************/
/*         A501/1 -> A501/ 1                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A501 and @va_sens_in_o = 1 )
begin
if (@va_autoroute_in_d = @vl_A501  and @va_sens_in_d = 1 )
begin 
  select @va_destination=@vl_AUBAGNE 
  select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
  select @va_presence_bifurcation_out = 0
end

/********************************************************************/
/*         A501 / 1  -> A8 / 1                                      */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 1 and (@va_PR_in_d>30701 or @va_PR2_in_d>30701))
begin
    if (@va_PR_in_d <> @va_PR2_in_d )
    begin
	if @va_PR_in_d <30701
	begin
	    select @va_PR_in_d=30700
	end
    end
    select @va_distance_out =  (5375-@va_PR_in_o) + (20900) + (@va_PR_in_d-30700) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_NICE
end

/********************************************************************/
/*         A501 / 1  -> A8 / 2                                      */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A8 and @va_sens_in_d = 2 and (@va_PR_in_d<30701 or @va_PR2_in_d<30701))
begin
    if (@va_PR_in_d <> @va_PR2_in_d )
    begin
	if @va_PR_in_d >30700
	begin
	    select @va_PR_in_d=30700
	end
    end
    select @va_distance_out =  (5375-@va_PR_in_o) + (20900) + (30700-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AIX
  
end

/********************************************************************/
/*         A501 / 1  -> A52 / 2                                     */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 2 and (@va_PR_in_d<20901 or @va_PR2_in_d<20901))
begin
    if (@va_PR_in_d <> @va_PR2_in_d )
    begin
	if @va_PR_in_d >20900
	begin
	    select @va_PR_in_d=20900
	end
    end
    select @va_distance_out =  (5375-@va_PR_in_o) + (20900-@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_BIF_A8
end

/********************************************************************/
/*         A501 / 1  -> A51 / 1                                     */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A51 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (5375-@va_PR_in_o) + 20900 + (30700-17900) + @vl_traversee_aix + (@va_PR_in_d-23310) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_LES_ALPES
end

/********************************************************************/
/*         A501 / 1  -> A500 / 1                                    */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A500 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (5375-@va_PR_in_o) + 20900 + (207400-30700) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_MONACO
end

/********************************************************************/
/*         A501 / 1  -> A520 / 1                                    */
/********************************************************************/
else 
if ( @va_autoroute_in_d = @vl_A520 and @va_sens_in_d = 1 )
begin
    select @va_distance_out =  (5375-@va_PR_in_o) + (20900-17000) + (@va_PR_in_d) 
    select @va_presence_bifurcation_out = 1
    select @va_destination=@vl_AURIOL
end
end
/********************************************************************/
/*         A501/2 -> A501/ 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A501 and @va_sens_in_o = 2 )
begin
if (@va_autoroute_in_d = @vl_A501  and @va_sens_in_d = 2 )
begin 
  select @va_destination=@vl_MARSEILLE 
  select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
  select @va_presence_bifurcation_out = 0
end
end
/********************************************************************/
/*         A520/1 -> A520/1                                         */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A520 and @va_sens_in_o = 1 )
begin
  if (@va_autoroute_in_d = @vl_A520  and @va_sens_in_d = 1 )
  begin 
    select @va_destination=@vl_AURIOL 
    select @va_distance_out =  @va_PR_in_d - @va_PR_in_o
    select @va_presence_bifurcation_out = 0
  end
end
/********************************************************************/
/*         A520/2 -> A520/ 2                                        */
/********************************************************************/
else
if ( @va_autoroute_in_o = @vl_A520 and @va_sens_in_o = 2 )
begin
  if (@va_autoroute_in_d = @vl_A520  and @va_sens_in_d = 2 )
  begin 
    select @va_destination=@vl_AUBAGNE 
    select @va_distance_out =  @va_PR_in_o - @va_PR_in_d
    select @va_presence_bifurcation_out = 0
  end
/********************************************************************/
/*         A520 / 2  -> A52 / 1                                     */
/********************************************************************/
  else 
  if ( @va_autoroute_in_d = @vl_A52 and @va_sens_in_d = 1 )
  begin
    if @va_PR_in_d < 17000 select @va_distance_out =  99999
    else
    begin
      select @va_distance_out =  (@va_PR_in_o) + (@va_PR_in_d-17000) 
      select @va_presence_bifurcation_out = 1
      select @va_destination=@vl_AUBAGNE
    end
  end

/********************************************************************/
/*         A520 / 2  -> A50 / 1                                     */
/********************************************************************/
  else 
  if ( @va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 1 )
  begin
    if @va_PR_in_d < 25847 select @va_distance_out =  99999
    else
    begin
      select @va_distance_out =  (@va_PR_in_o) + (26077-17000) + (@va_PR_in_d-25847) 
      select @va_presence_bifurcation_out = 1
      select @va_destination=@vl_TOULON
    end
  end

/********************************************************************/
/*         A520 / 2  -> A501 / 2                                    */
/********************************************************************/
  else 
  if ( @va_autoroute_in_d = @vl_A501 and @va_sens_in_d = 2 )
  begin
    if @va_PR_in_d < 25847 select @va_distance_out =  99999
    else
    begin
      select @va_distance_out =  (@va_PR_in_o) + (26077-17000) + (5375-@va_PR_in_d) 
      select @va_presence_bifurcation_out = 1
      select @va_destination=@vl_MARSEILLE
    end
  end
end
/********************************************************************/
/*         cas inconnu                                              */
/********************************************************************/
else
begin        
  select @va_distance_out =  99999            
  select @va_presence_bifurcation_out = 1
end
   return XDC_OK
go 
	 
