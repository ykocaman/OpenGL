class City{
  
  PShape shape;
  int id;
  int population;
  int student;
  int teacher;
  String name;
  
    
  City(int id){
    this.id = id;
    this.name = getCityName(id);
  }
  
  String getShapeId(){
     return "TR-" + (this.id<10 ? "0" : "")+this.id;
  }
  
  int getPercent(){
     return this.student == 0 ? 0 : (int)(this.population / this.student);
  }
  
  void setShape(PShape shape){
    this.shape = shape;
  }
  
  PShape getShape(){
    return this.shape;
  }
  
  void setPopulation(int population){
    this.population = population;
  }
  
  int getPopulation(){
    return this.population;
  }
  
  void setStudent(int student){
    this.student = student;
  }
  
  int getStudent(){
    return this.student;
  }
  
   void setTeacher(int teacher){
    this.teacher = teacher;
  }
  
  int getTeacher(){
    return this.teacher;
  }
  
  
  String getName(){
     return this.name;
  }
  
  int getId(){
    return this.id;
  }
  
  PVector getCenterPoints(){
     
    float minX,maxX,minY,maxY;
    maxX=maxY=0;
    minX=minY=1052; 
    
    for(int i=0;i<this.shape.getVertexCount();i++){ 
      
      if(minX > this.shape.getVertex(i).x)
        minX = this.shape.getVertex(i).x;
      else if(maxX < this.shape.getVertex(i).x)
        maxX = this.shape.getVertex(i).x;
        
       if(minY > this.shape.getVertex(i).y)
        minY = this.shape.getVertex(i).y;
      else if(maxY < this.shape.getVertex(i).y)
        maxY = this.shape.getVertex(i).y;
    } 
    
    PVector   vector = new PVector();
    
    vector.x = minX + (maxX - minX) / 2.5;
    vector.y = minY + (maxY - minY) / 2;
    
    return vector;
  }
}
