sample1 <- read.csv("C:/Users/Fati/Desktop/whole sample1with GAP&MGAP.csv")
str(sample1)

#calcuate triss 

mydat<-subset(sample1,select = c(new.rts,new.iss,idade,mecanismo_i_1))
str(mydat)

#a=rts_result  b=iss_result c=age 

myfunc<-function(a,b,cc,mm){
  
  #the ccoefficients b0-b3 for (blunt and penetrating )
  
  sharpb0 = -2.5355 ;  sharpb1 = 0.9934;  sharpb2 = -0.0651;  sharpb3 = -1.1360;
  
  bluntb0 = -0.4499 ; bluntb1 = 0.8085; bluntb2 = -0.0835; bluntb3 = -1.7430;
  
  trissblunthold=NULL ;trisssharphold =NULL;agei = NULL; ps = NULL;triss=NULL;mmi=NULL;
  
  for (i in 1:nrow(mydat)){ 
    
    if (cc[i]>=0 && cc[i]<=54){
      
      agei=0
      
    }else if (cc[i]>=55 && cc[i]<=120){
      
      agei=1
      
    }else{
      
      print("patient age out of rang")
      
      
    }
    
    if (mm[i]==1){
      
      trisssharphold[i]<-(bluntb0 + bluntb1*a[i] + bluntb2*b[i] + bluntb3*agei)
      
      
    }else{
      
      
      
      trisssharphold[i]<- (sharpb0 + sharpb1*a[i] + sharpb2*b[i]+ sharpb3*agei)
      
      
    }
    
    #multi return values
    
    #retval[i]<-list(trisssharphold[i],trissblunthold[i])
    ps[i] = round(1/(1+ exp(-trisssharphold[i]))*100)
    
    
  } 
  #test = myrts(dat$glasgow_resultado,dat$pas,dat$fr)
  return(ps)
  
  
}

#myfunc(mydat$rts_resultado,mydat$iss_resultado,mydat$idade)

m11<-myfunc(mydat$new.rts, mydat$new.iss ,mydat$idade,mydat$mecanismo_i_1)

aaa <- read.csv("C:/Users/Fati/Desktop/aaa.csv")
write.csv(m11, file="C:/Users/Fati/Desktop/aaa.csv")
write.table(m11, file="C:/Users/Fati/Desktop/aaa.csv", sep = ",", col.names = NA, qmethod = "double")






