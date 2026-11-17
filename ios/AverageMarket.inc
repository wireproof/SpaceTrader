// Native port actions and route planning retain the original transaction rules.
extern long NativeAveragePrice(int good,int system);
extern void NativeCargoQuantity(int good,Boolean buying);
extern Boolean SystemInformationFormHandleEvent(EventPtr);
extern Boolean AveragePricesFormHandleEvent(EventPtr);
static void nativeNotice(NSString*title,NSString*message){UIAlertController*a=[UIAlertController alertControllerWithTitle:title message:message preferredStyle:UIAlertControllerStyleAlert];[a addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];[controller presentViewController:a animated:YES completion:nil];}
static void nativePortAction(int control){
 if(control==SystemInformationNewsButton){nativePortInfo=YES;dirty=YES;return;}
 if(control==SystemInformationMercenaryForHireButton){nativeCrewFocus=YES;nativeMenu(MenuCommandPersonnelRoster);return;}
 FormPtr previous=active,temp=FrmInitForm(SystemInformationForm);NSArray*old=[textRuns copy];active=temp;EventType update={.eType=frmUpdateEvent};SystemInformationFormHandleEvent(&update);Object*special=FrmGetObjectPtr(temp,FrmGetObjectIndex(temp,SystemInformationSpecialButton));BOOL available=special&&!special->hidden;active=previous;FrmDeleteForm(temp);textRuns=[old mutableCopy];dirty=YES;
 if(available){CurForm=SpecialEventForm;FrmGotoForm(CurForm);}else nativeNotice(@"Special",@"No special event is available in this system.");
}
static void nativeReadNews(void){nativePortInfo=NO;dashboardRoute(MenuCommandSystemInformation,SystemInformationForm,SystemInformationNewsButton);dirty=YES;}
static void nativePortSheet(CGFloat width,CGFloat height){
 for(UIView*v in nativeSurface.subviews)v.userInteractionEnabled=NO;
 UIView*shade=[UIView new];shade.frame=CGRectMake(0,0,width,height);shade.backgroundColor=[UIColor.blackColor colorWithAlphaComponent:.28];[nativeSurface addSubview:shade];CGFloat w=MIN(width-24,520),h=MIN(height-24,520);UIScrollView*sheet=[UIScrollView new];sheet.frame=CGRectMake((width-w)/2,MAX(12,(height-h)/2),w,h);sheet.layer.cornerRadius=20;sheet.backgroundColor=UIColor.secondarySystemGroupedBackgroundColor;sheet.tag=49100;[nativeSurface addSubview:sheet];
 nativeLabel(sheet,@"System Info",CGRectMake(16,12,w-32,32),YES);
 SOLARSYSTEM s=CURSYSTEM;NSArray*names=@[@"Name",@"Size",@"Tech Level",@"Government",@"Resources",@"Police",@"Pirates"];NSArray*values=@[str(SolarSystemName[s.NameIndex]),str(SystemSize[s.Size]),str(TechLevel[s.TechLevel]),str(Politics[s.Politics].Name),str(SpecialResources[s.SpecialResources]),str(Activity[Politics[s.Politics].StrengthPolice]),str(Activity[Politics[s.Politics].StrengthPirates])];
 for(int i=0;i<7;i++){nativeLabel(sheet,names[i],CGRectMake(12,52+i*38,(w-32)/2,34),YES);nativeLabel(sheet,values[i],CGRectMake(w/2,52+i*38,w/2-12,34),NO);}
 nativeLabel(sheet,str(Status[CURSYSTEM.Status]),CGRectMake(16,320,w-32,54),NO).font=[UIFont systemFontOfSize:14];
 UIButton*news=nativeButton(sheet,@"News",CGRectMake(16,382,w-32,44),^{nativeReadNews();});news.tag=49101;
 nativeButton(sheet,@"Done",CGRectMake(16,438,w-32,44),^{nativePortInfo=NO;dirty=YES;});sheet.contentSize=CGSizeMake(w,494);nativeSurface.contentSize=CGSizeMake(width,height);nativeSurface.contentOffset=CGPointZero;
}
static void nativeAverageAction(int control){EventType e={.eType=ctlSelectEvent};e.data.ctlSelect.controlID=control;AveragePricesFormHandleEvent(&e);dirty=YES;}
static void nativeAverageTrade(int good,BOOL buy){FormPtr previous=active,temp=FrmInitForm(buy?BuyCargoForm:SellCargoForm);active=temp;NativeCargoQuantity(good,buy);active=previous;FrmDeleteForm(temp);EventType e={.eType=frmUpdateEvent};AveragePricesFormHandleEvent(&e);dirty=YES;}
static void nativeAverageMarket(CGFloat width,CGFloat height){
 CGFloat w=width-24,y=8;nativeLabel(nativeSurface,@"Average Prices",CGRectMake(12,y,w,40),YES).font=[UIFont systemFontOfSize:28 weight:UIFontWeightBold];y+=48;
 UIView*header=nativeCard(nativeSurface,CGRectMake(12,y,w,118));nativeLabel(header,str(SolarSystemName[SolarSystem[WarpSystem].NameIndex]),CGRectMake(64,8,w-128,34),YES);
 nativeButton(header,@"‹",CGRectMake(12,6,44,40),^{nativeAverageAction(AveragePricesScrollBackButton);});nativeButton(header,@"›",CGRectMake(w-56,6,44,40),^{nativeAverageAction(AveragePricesScrollFwdButton);});
 nativeLabel(header,SolarSystem[WarpSystem].Visited?str(SpecialResources[SolarSystem[WarpSystem].SpecialResources]):@"Special resources unknown",CGRectMake(12,42,w-24,24),NO).font=[UIFont systemFontOfSize:14];
 nativeLabel(header,[NSString stringWithFormat:@"Trades at %@ · %ld credits\nCargo %d / %d",str(SolarSystemName[CURSYSTEM.NameIndex]),Credits,FilledCargoBays(),TotalCargoBays()],CGRectMake(12,68,w-24,44),NO).font=[UIFont systemFontOfSize:14];y+=130;
 nativeButton(nativeSurface,PriceDifferences?@"Show Absolute Prices":@"Show Price Differences",CGRectMake(12,y,w,42),^{nativeAverageAction(AveragePricesAbsolutePricesButton);});y+=50;
 nativeLabel(nativeSurface,PriceDifferences?@"Target estimate − local buy price":@"Estimated prices at target system",CGRectMake(12,y,w,28),NO).font=[UIFont systemFontOfSize:13];y+=34;
 BOOL wide=w>=650;CGFloat gap=12,cw=wide?(w-gap)/2:w;
 for(int i=0;i<MAXTRADEITEM;i++){CGFloat x=12+(wide?i%2*(cw+gap):0),cy=y+(wide?i/2:i)*104;UIView*row=nativeCard(nativeSurface,CGRectMake(x,cy,cw,96));cargoArtwork(row,i,CGRectMake(12,8,32,32));
 nativeLabel(row,str(Tradeitem[i].Name),CGRectMake(52,4,cw-64,24),YES);long price=NativeAveragePrice(i,WarpSystem),diff=price-BuyPrice[i];NSString*estimate=price<=0||(PriceDifferences&&BuyPrice[i]<=0)?@"—":PriceDifferences?[NSString stringWithFormat:@"%+ld cr",diff]:[NSString stringWithFormat:@"%ld cr",price];
 nativeLabel(row,[NSString stringWithFormat:@"%@ · %d aboard · %d available",estimate,Ship.Cargo[i],CURSYSTEM.Qty[i]],CGRectMake(48,28,cw-60,22),NO).font=[UIFont systemFontOfSize:13];CGFloat bw=(cw-32)/2;
 for(int b=0;b<2;b++){BOOL buy=b==1;long local=buy?BuyPrice[i]:SellPrice[i];UIButton*button=nativeButton(row,local>0?[NSString stringWithFormat:@"%@ %ld cr",buy?@"Buy":@"Sell",local]:buy?@"Not Sold":@"Not Bought",CGRectMake(12+b*(bw+8),52,bw,40),^{nativeAverageTrade(i,buy);});button.tag=49200+b*10+i;button.titleLabel.font=[UIFont systemFontOfSize:14 weight:UIFontWeightSemibold];button.enabled=local>0&&(buy?(CURSYSTEM.Qty[i]>0&&FilledCargoBays()<TotalCargoBays()&&ToSpend()>=local):Ship.Cargo[i]>0);}
 }
 y+=(wide?5:10)*104;CGFloat bw=(w-8)/2;nativeButton(nativeSurface,@"System Info",CGRectMake(12,y,bw,44),^{nativeAverageAction(AveragePricesSystemInfoButton);});nativeButton(nativeSurface,@"Warp",CGRectMake(20+bw,y,bw,44),^{nativeAverageAction(AveragePricesWarpButton);});nativeSurface.contentSize=CGSizeMake(width,MAX(height,y+56));
}
