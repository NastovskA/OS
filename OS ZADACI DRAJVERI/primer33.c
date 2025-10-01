
char mem[6*1024];
char dop[4*1024];
int w;
int flag;

static int r_init(void){
w=0;
flag=0;
register_chrdev(60,"primer1",&myfops);
return 0;
}
static int r_exit(void){

unregister_chrdev(60,"primer1");
}
ssize_t read(file *f,char *buff,size_t count,...){
int z;
if(flag==0){
    if(w>=count){
        z=count;
    }else {
        z=w;
    }
 copy_to_user(buff,(mem+w)-z,z);
 w-=z;   
}
else {
if(w>=count){
    z=count;
copy_to_user(buff,(dop+w)-z,z);
w-=z;
}
else {
    copy_to_user(buff,dop,w);
    z=count-w;
    w=6*1024;
    copy_to_user(buff,mem+w-z,z);
    w-=z;
    flag=0;
}    
}
}


}
ssize_t write(file *f,char *buff,size_t count,...){
    int z;
    if(flag==0){
        if((6*1024-w)>=count){
        copy_from_user(mem+w,buff,count);
        w+=count;
        }else {
            z=6*1024-w
            if(z+4*1024<count){
                return 0;
            }
            copy_from_user(mem+w,buff,z);
            flag=1;
            copy_from_user(dop,buff+z,count-z);
            w=count-z;
        }
    }else {
if(4*1024-w>=count){
copy_from_user(dop+w,buff,count);
w+=count;
}else {
    return 0;
}
    }
}