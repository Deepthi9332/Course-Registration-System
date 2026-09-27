#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>

using namespace std ;

class student{

    public:

    string ID;
    string name;
    int year;
    int no_of_comp_courses;
    vector<string>comp_course_list;
    vector<string>curr_course_list;
    unordered_map<string,int>comp_course_map;

    student(){

        name=ID="";
        year=no_of_comp_courses=0;
        comp_course_list=curr_course_list={};
    }

    student(string id,string nam,int yr,int num,vector<string>comp){

        ID=id;
        name=nam;
        year=yr;
        no_of_comp_courses=num;
        comp_course_list=comp;
        curr_course_list={};
    }

};

class course{

    public:

    string ID;
    string name;
    int credit;
    int capacity;
    char slot;
    int no_of_courses;
    vector<string>prerequisites;
    vector<string>studs;
    vector<string>waiting_list_student;

    course(){

        ID=name="";
        credit=capacity=slot=no_of_courses=0;
        prerequisites=studs={};
    }

    course(string id,string nam,int cre,int cap,char slo,int num,vector<string>pre){

        ID=id;
        name=nam;
        credit=cre;
        capacity=cap;
        slot=slo;
        no_of_courses=num;
        prerequisites=pre;
        studs={};
    }

};

void student_add(vector<student*>& st,vector<course*>& cou,unordered_map<string,int>& student_map,unordered_map<string,int>& course_map,student* &pupil){
    
    string student_id,student_name,course_name;
    int student_year_of_study,number_of_completed_courses;
    vector<string>completed_courses;

    cin>>student_id>>student_name>>student_year_of_study>>number_of_completed_courses;

    for(int i=0;i<number_of_completed_courses;i++){

        cin>>course_name;
        completed_courses.push_back(course_name);
    }

    if(student_map.find(student_id)!=student_map.end())return; 

    pupil=new student(student_id,student_name,student_year_of_study,number_of_completed_courses,completed_courses);
    
    for(int i=0;i<number_of_completed_courses;i++)  pupil->comp_course_map[completed_courses[i]]=i;

    st.push_back(pupil);
    student_map[st[st.size()-1]->ID]=st.size()-1;

    return;

}

bool pre_requisites_exist(vector<course*>& cou,unordered_map<string,int>& course_map,vector<string>& prereqs){

    for(int i=0;i<prereqs.size();i++){

        if(course_map.find(prereqs[i])==course_map.end())return false;
    }

    return true;
}

bool cyclic_dependency(string course_id,vector<string>& prereqs,vector<course*>& cou,unordered_map<string,int>& course_map){

    for(int i=0;i<(prereqs.size());i++){

        course* sub=cou[course_map[prereqs[i]]];

        for(int j=0;j<(sub->prerequisites.size());j++){

            if(sub->prerequisites[j]==course_id)return true;
        }
    }

    return false;
}

void course_add(vector<student*>& st,vector<course*>& cou,unordered_map<string,int>& student_map,unordered_map<string,int>& course_map,course* &subject){
    
    string course_id,course_name,prereq_course;
    int course_credits,course_capacity,number_of_prerequisites;
    char course_slot;
    vector<string>prerequisite_courses;

    cin>>course_id>>course_name>>course_credits>>course_capacity>>course_slot>>number_of_prerequisites;

    for(int i=0;i<number_of_prerequisites;i++){

        cin>>prereq_course;
        prerequisite_courses.push_back(prereq_course);
    }

    if(course_map.find(course_id)!=course_map.end())return; 
    if(!pre_requisites_exist(cou,course_map,prerequisite_courses))return; 
    if(cyclic_dependency(course_id,prerequisite_courses,cou,course_map))return;

    subject=new course(course_id,course_name,course_credits,course_capacity,course_slot,number_of_prerequisites,prerequisite_courses);

    cou.push_back(subject);
    course_map[cou[cou.size()-1]->ID]=cou.size()-1;

    return;
    
}

bool student_exist(unordered_map<string,int>& student_map,string &student_id){

    return student_map.find(student_id)!=student_map.end();
}

bool course_exist(unordered_map<string,int>& course_map,string &course_id){

    return course_map.find(course_id)!=course_map.end();
}

bool prerequisite_eligible(course* &sub,student* &child){

    for(int i=0;i<(sub->no_of_courses);i++)

        if((child->comp_course_map.find(sub->prerequisites[i]))==child->comp_course_map.end())

            return false;

    return true;
}

bool slot_free(course* &sub,student* &child,vector<course*>& cou,unordered_map<string,int>& course_map){

    for(int i=0;i<(child->curr_course_list.size());i++)

        if(cou[course_map[(child->curr_course_list[i])]]->slot==sub->slot)

            return false;
        
    return true;
}

bool capacity_exist(course* &sub){
    
    return sub->capacity>0;
}

bool is_student_already_in_course(string student_id,course* sub){

    for(int i=0;i<(sub->studs).size();i++){

        if(sub->studs[i]==student_id)return false;
    }

    return true;
}

void enrollment(vector<student*>& st,vector<course*>& cou,unordered_map<string,int>& student_map,unordered_map<string,int>& course_map,student* &pupil,course* &subject){
    
    string student_id,course_id;
    cin>>student_id>>course_id;

    if(student_exist(student_map,student_id)&&(course_exist(course_map,course_id))){
        
        course* sub=cou[course_map[course_id]];
        student* child=st[student_map[student_id]];

        if(!is_student_already_in_course(student_id,sub))return; 

        if(prerequisite_eligible(sub,child)){
            
            if(slot_free(sub,child,cou,course_map)&&capacity_exist(sub)){
                
                child->curr_course_list.push_back(course_id);
                sub->capacity--;
                sub->studs.push_back(student_id);

            }

            else sub->waiting_list_student.push_back(student_id);

        }

    }

    return;

}

void print(vector<course*>& cou,unordered_map<string,int>& course_map){

    string course_id;
    cin>>course_id;

    if(course_map.find(course_id)==course_map.end())    cout<<"Invalid Course "<<course_id<<endl;

    else{

        course* sub=cou[course_map[course_id]];
    
        if(sub->studs.size()==0);

        else{

            cout<<"Enrolled students in "<<course_id<<":"<<endl;
            for(int i=0;i<(sub->studs.size());i++)  cout<<(sub->studs[i])<<endl;

        }

    }
    
    return;

}

void drop(vector<student*>& st,vector<course*>& cou,unordered_map<string,int>& student_map,unordered_map<string,int>& course_map){

    string student_id,course_id;
    cin>>student_id>>course_id;

    if(student_map.find(student_id)==student_map.end()||course_map.find(course_id)==course_map.end())return; 

    student* kid=st[student_map[student_id]];
    course* topic=cou[course_map[course_id]];

    topic->capacity++;

    int i;
    for(i=0;i<(topic->studs.size());i++)    if(topic->studs[i]==student_id) break;

    topic->studs.erase(topic->studs.begin()+i);

    int j;
    for(j=0;j<(kid->curr_course_list.size());j++)   if(kid->curr_course_list[i]==course_id) break;

    kid->curr_course_list.erase(kid->curr_course_list.begin()+j);

    int siz=topic->waiting_list_student.size();

    if(siz==0);

    else{

        student* tot;
        int index=0;

        while(!(topic->waiting_list_student.empty())){

            string champ = topic->waiting_list_student[index];

            (topic->waiting_list_student).erase(topic->waiting_list_student.begin()+index);

            tot=st[student_map[champ]];

            if(slot_free(topic,tot,cou,course_map)&&capacity_exist(topic)){
                
                tot->curr_course_list.push_back(course_id);
                topic->capacity--;
                topic->studs.push_back(champ);

                return;

            }

        }

    }

    return;

}

int main () {

    int n;
    cin>>n;

    string s;

    vector<student*>st;
    vector<course*>cou;

    unordered_map<string,int>student_map;
    unordered_map<string,int>course_map;

    student* pupil;
    course* subject;

    for(int i=0;i<n;i++){

        cin>>s;

        if(s=="add_student")    student_add(st,cou,student_map,course_map,pupil);

        else if(s=="add_course")    course_add(st,cou,student_map,course_map,subject);

        else if(s=="enroll")    enrollment(st,cou,student_map,course_map,pupil,subject);

        else if(s=="print")     print(cou,course_map);

        else if(s=="drop")      drop(st,cou,student_map,course_map);

        else ;
    }

    return 0;

}

