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

    for(int k=0;k<n;k++){

        cin>>s;

        if(s=="add_student"){

            string student_id,student_name,course_name;
            int student_year_of_study,number_of_completed_courses;
            vector<string>completed_courses;

            cin>>student_id>>student_name>>student_year_of_study>>number_of_completed_courses;

            for(int i=0;i<number_of_completed_courses;i++){

                cin>>course_name;
                completed_courses.push_back(course_name);
            }

            if(student_map.find(student_id)!=student_map.end())continue;

            pupil=new student(student_id,student_name,student_year_of_study,number_of_completed_courses,completed_courses);
            
            for(int i=0;i<number_of_completed_courses;i++){

                pupil->comp_course_map[completed_courses[i]]=i;
            }

            st.push_back(pupil);
            
            student_map[st[st.size()-1]->ID]=st.size()-1;

        }

        else if(s=="add_course"){

            string course_id,course_name,prereq_course;
            int course_credits,course_capacity,number_of_prerequisites;
            char course_slot;
            vector<string>prerequisite_courses;

            cin>>course_id>>course_name>>course_credits>>course_capacity>>course_slot>>number_of_prerequisites;

            for(int i=0;i<number_of_prerequisites;i++){

                cin>>prereq_course;
                prerequisite_courses.push_back(prereq_course);
            }

            if(course_map.find(course_id)!=course_map.end())continue;
            
            bool b=1;

            for(int i=0;i<(prerequisite_courses.size());i++){

                if(course_map.find(prerequisite_courses[i])==course_map.end()){

                    b=0;
                    break;
                }
            }

            if(!b)continue;

            subject=new course(course_id,course_name,course_credits,course_capacity,course_slot,number_of_prerequisites,prerequisite_courses);
            cou.push_back(subject);

            course_map[cou[cou.size()-1]->ID]=cou.size()-1;

        }

        else if(s=="enroll"){

            string student_id,course_id;
            cin>>student_id>>course_id;

            if((student_map.find(student_id)!=student_map.end())&&(course_map.find(course_id)!=course_map.end())){

                course* sub=cou[course_map[course_id]];
                student* child=st[student_map[student_id]];

                bool eligible=1;

                for(int i=0;i<(sub->no_of_courses);i++){

                    if((child->comp_course_map.find(sub->prerequisites[i]))==child->comp_course_map.end()){

                        eligible=0;
                        break;
                    }
                }

                if(eligible){

                    bool slotfree=1;

                    for(int i=0;i<(child->curr_course_list.size());i++){

                        if(cou[course_map[(child->curr_course_list[i])]]->slot==sub->slot){

                            slotfree=0;
                            break;
                        }
                    }

                    if(slotfree){

                        if(sub->capacity>0){

                            child->curr_course_list.push_back(course_id);
                            sub->capacity--;
                            sub->studs.push_back(student_id);
                        }

                    }

                }

            }

        }

        else if(s=="print"){

            string course_id;
            cin>>course_id;

            if(course_map.find(course_id)==course_map.end()){

                cout<<"Invalid Course "<<course_id<<endl;
                continue;
            }

            course* sub=cou[course_map[course_id]];
            
            if(sub->studs.size()==0);

            else{

                cout<<"Enrolled students in "<<course_id<<":"<<endl;

                for(int i=0;i<(sub->studs.size());i++){

                    cout<<(sub->studs[i])<<endl;
                }
            }
        }
    }

    return 0;
}

