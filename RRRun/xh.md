int main()
{
   int n,s=1,max=1;
   scanf("%d",&n);
   int a[10000];
   for(int i=0;i<n;i++)
   {
   	scanf("%d",&a[i]);
	
    if(a[i]==a[i-1]+1)
   {
   	s=s+1;
   }
   else 
   {
   s=1;
   }
   if(max<s)
   {
   	max=s;
   }
   }
   printf("%d",max);
   return 0;
}