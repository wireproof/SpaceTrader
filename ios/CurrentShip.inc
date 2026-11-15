// Current vessel overview, using the live game state and original navigation.
static void nativeCurrentShip(CGFloat width,CGFloat height){
 CGFloat w=width-24;BOOL wide=width>=700;CGFloat cw=wide?(w-12)*.5:w;
 nativeLabel(nativeSurface,@"Current Ship",CGRectMake(12,8,w,40),YES).font=[UIFont systemFontOfSize:28 weight:UIFontWeightBold];
 UIView*v=nativeCard(nativeSurface,CGRectMake(12,56,cw,wide?364:300));v.tag=43002;
 UIImageView*art=[[UIImageView alloc]initWithImage:hdShip(Ship.Type)];art.contentMode=UIViewContentModeScaleAspectFit;art.frame=CGRectMake(cw*.48,8,cw*.48,wide?142:110);[v addSubview:art];
 nativeLabel(v,str(Shiptype[Ship.Type].Name),CGRectMake(16,22,cw*.45-16,40),YES).font=[UIFont systemFontOfSize:26 weight:UIFontWeightBold];
 nativeLabel(v,ScarabStatus==3?@"Hardened hull":@"Your ship",CGRectMake(16,68,cw*.45-16,48),NO).textColor=UIColor.secondaryLabelColor;
 shipMeter(v,@"Fuel (pc)",GetFuel(),GetFuelTanks(),wide?164:128,cw,UIColor.systemBlueColor,NO);
 shipMeter(v,@"Hull",Ship.Hull,GetHullStrength(),wide?224:184,cw,UIColor.systemGreenColor,NO);
 shipMeter(v,@"Cargo bays",FilledCargoBays(),TotalCargoBays(),wide?284:240,cw,UIColor.systemOrangeColor,YES);
 shipSlots(nativeSurface,CGRectMake(wide?24+cw:12,wide?56:368,cw,wide?364:252));
 CGFloat y=wide?432:632;NSMutableArray*items=[NSMutableArray new];
 for(int k=0;k<3;k++){int count=k==0?MAXWEAPON:k==1?MAXSHIELD:MAXGADGET;for(int j=0;j<count;j++){int t=k==0?Ship.Weapon[j]:k==1?Ship.Shield[j]:Ship.Gadget[j];if(t<0)continue;NSString*name=k==0?str(Weapontype[t].Name):k==1?str(Shieldtype[t].Name):str(Gadgettype[t].Name);int icon=k==0?(t<3?t:-1):k==1?(t<2?t+3:-1):(t<5?t+5:-1);[items addObject:@{@"name":name,@"icon":@(icon)}];}}
 if(EscapePod)[items addObject:@{@"name":@"Escape pod",@"icon":@(-1)}];
 UIView*loadout=dashboardPanel(nativeSurface,@"Installed equipment",CGRectMake(12,y,w,MAX(88,44+items.count*64)));loadout.tag=43003;
 if(!items.count)nativeLabel(loadout,@"No equipment installed",CGRectMake(16,42,w-32,30),NO);
 for(int i=0;i<items.count;i++){NSDictionary*item=items[i];int icon=[item[@"icon"]intValue];UIImageView*im=[[UIImageView alloc]initWithImage:icon>=0?equipmentArt(icon):[UIImage systemImageNamed:@"gearshape"]];im.contentMode=UIViewContentModeScaleAspectFit;im.frame=CGRectMake(16,42+i*64,64,56);[loadout addSubview:im];nativeLabel(loadout,item[@"name"],CGRectMake(92,44+i*64,w-108,48),YES);}
 y+=loadout.frame.size.height+12;NSArray*titles=@[@"Status",@"Quests",@"Special cargo"];int ids[]={CurrentShipStatusButton,CurrentShipQuestsButton,CurrentShipSpecialButton};CGFloat bw=(w-16)/3;
 for(int i=0;i<3;i++){int c=ids[i];UIButton*b=nativeButton(nativeSurface,titles[i],CGRectMake(12+i*(bw+8),y,bw,44),^{Object*o=FrmGetObjectPtr(active,FrmGetObjectIndex(active,c));if(o)nativeControl(o);});b.tag=30000+c;}
 nativeSurface.contentSize=CGSizeMake(width,MAX(height,y+56));
}
