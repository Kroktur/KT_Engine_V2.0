#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include <deque>
#include <Tools/Random.h>
#include "Tools/Chrono.h"
#include "Core/QuadTree.h"
#include <Core/CompositeCrtp.h> 
static  KT::Random rd;
static KT::Vector2F Amin{ -1920  * 5,-1080 *5 };
static KT::Vector2F Amax{ 1920 *5 ,1080 *5};

static sf::Color RainbowColor(float dt,float speed)
{

    float r = (std::sin(speed * dt + 0.f) + 1.f) * 0.5f; 
    float g = (std::sin(speed * dt + 2.f) + 1.f) * 0.5f;
    float b = (std::sin(speed * dt + 4.f) + 1.f) * 0.5f;

    return sf::Color(static_cast<std::uint8_t>(r * 255),
        static_cast<std::uint8_t>(g * 255),
        static_cast<std::uint8_t>(b * 255));
}


struct NodeGO : public KT::ExcludeRootCRTP<NodeGO,IGO, NodeGO>
{
	
};

class Circle : public IGO, public KT::CompositeCRTP<Circle,IGO, NodeGO>
{
public:
    Circle(KT::IComposite<IGO, NodeGO>* parent) : circle(5), KT::CompositeCRTP<Circle, IGO, NodeGO>(parent)
	{
        static int tid = globId++;
        id = tid;
        circle.setPosition({ rd.getRandomNumber<float>(Amin.x,Amax.x), rd.getRandomNumber<float>(Amin.y,Amax.y) });
        speed = rd.getRandomNumber< float>(0.01f, 1.0f);
        KT::Vector2F Amin{ circle.getPosition().x,circle.getPosition().y };
        KT::Vector2F Amax{ circle.getPosition().x + circle.getRadius(),circle.getPosition().y + circle.getRadius() };
        AABB = { Amin,Amax };

        auto t = rd.getRandomNumber<int>(0,1);
        if (t == 0)
            moovingEntity = true;
    }
     void Input() override{}
     void Update(float dt) override
    {

         if (!moovingEntity)
           return;
         static float speed = 1000;
         static float ymin = Amin.y + circle.getRadius() + 1 ;
         static float ymax = Amax.y - circle.getRadius() - 1;

         auto y = circle.getPosition().y + speed * dt;
        if ( y > ymax)
        {
            y = ymax;
            remove = true;
        }
        circle.setPosition({ circle.getPosition().x,y });
       /*  totalTime += dt;
         circle.setFillColor(sf::Color::Red);*/
    }
    box UpdateAABB()
    {
        if (moovingEntity)
        {
            KT::Vector2F Amin{ circle.getPosition().x,circle.getPosition().y };
            KT::Vector2F Amax{ circle.getPosition().x + circle.getRadius(),circle.getPosition().y + circle.getRadius() };
            AABB = { Amin,Amax };
        }
        return AABB;
    }
    bool isMoving()
    {
        return moovingEntity;
    }
     void Render(sf::RenderWindow* win) override
    {
         win->draw(circle);
    }
    box GetAABB()
    {
      return AABB;
    }
    bool NeedRemove()
    {
        
        return  remove;
    }
    void Remove()
    {
        isRemove = true;
    }
    bool IsRemove()
    {
        return isRemove;
    }
    int id;
    static int globId;

    bool operator==(const Circle& c) const
    {
        return  id == c.id;
    }
private:
    box AABB;
    bool remove = false;
    bool isRemove = false;
    sf::CircleShape circle;
    float speed;
    float totalTime = 0;
    bool moovingEntity = false;
};

int Circle::globId = 0;
int main()
{
    KT::Vector2F AminQuad = Amin - KT::Vector2F{10,10};
    KT::Vector2F AmaxQuad = Amax + KT::Vector2F{ 10,10 };


    Quad::QuadTree quadTree({ AminQuad,AmaxQuad });

    NodeGO n;



    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "SFML 3.0 Example");

    sf::View view(sf::FloatRect({ 0, 0 }, { 1920, 1080 }));


   
    float size = 10000;
    
    for (int i = 0 ; i < size; ++i)
    {
        auto circle = new Circle(&n);
        auto node = EntityInfo{ circle,circle->GetAABB() };
        quadTree.Insert(node);
    }
    
    KT::Chrono<float> UpdateDT;
    KT::Chrono<float>::Time updateFrameRate = KT::Chrono<float>::Time::CreateFromValue<KT::ratio<1>>(1.0f / 60.0f);
    KT::Chrono<float>::Time lastFrameUpdate = UpdateDT.GetElapsedTime();
    KT::Chrono<float> RenderDT;
    KT::Chrono<float>::Time renderFrameRate = KT::Chrono<float>::Time::CreateFromValue<KT::ratio<1>>(1.0f / 60.0f);
    KT::Chrono<float>::Time lastFrameRender = RenderDT.GetElapsedTime();


    sf::RectangleShape rec({20,20});
    rec.setPosition({ 400,400 });
    while (window.isOpen())
    {
        
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        if (UpdateDT.GetElapsedTime() - lastFrameUpdate > updateFrameRate)
        {
            n.ExecuteAction([&](IGO* g)
                {
                    g->Update(updateFrameRate.AsSeconds());
                    auto c = static_cast<Circle*>(g);
                    if (c->IsRemove())
                        return;
					if (c->isMoving())
					{
                        auto old = c->GetAABB();
                        auto newa = c->UpdateAABB();
                        if (c->NeedRemove())
                        {
	                        quadTree.Remove({ g,old });
                            c->Remove();
                        }
                        else
							 quadTree.Update({ g,old }, newa);
					}
                    
                });
         
            lastFrameUpdate = UpdateDT.GetElapsedTime();
            rec.rotate(sf::degrees(10) * updateFrameRate.AsSeconds());



            sf::Vector2f movement(0.f, 0.f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) movement.y -= 1000.0f * updateFrameRate.AsSeconds();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1000.0f * updateFrameRate.AsSeconds();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) movement.x -= 1000.0f * updateFrameRate.AsSeconds();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1000.0f * updateFrameRate.AsSeconds();

            view.move(movement);
        	window.setView(view);
            quadTree.UpdataMerge();
        }

       
        

        if (RenderDT.GetElapsedTime() - lastFrameRender > renderFrameRate)
        {


            window.clear(sf::Color::Black);
     /*       for (auto& c : m_shapes)
                c.Render(&window);*/
            std::vector<sf::RectangleShape> Quadrec;

            sf::Vector2f center = view.getCenter();
        	box box = {{center.x - 1920/2 , center.y - 1080 /2 },{center.x + 1920/2  , center.y + 1080/2 }};
           //box box = { Amin,Amax };

            std::vector <IGO*> vecToDraw;
				quadTree.ExecuteAction([&](Quad::QuadTree::Node* n)
                {
                    std::deque<Quad::QuadTree::Node*> allNode;
                    allNode.push_back(n);

                    while (!allNode.empty())
                    {
                        auto* current = allNode.front();
                        allNode.pop_front();

                        if (!current->aabb.Intersects(box))
                            continue;

                        if (current->IsSubDivided())
                        {
                            for (int i = 0; i < 4; ++i)
                                allNode.push_back(current->childNodes[i].get());
                            continue;
                        }
                        for (int i = 0; i < current->Size(); ++i)
                        {
                            auto it = std::find_if(vecToDraw.begin(), vecToDraw.end(), [&](IGO* e)
                                {
                                    return	e == current->m_entity[i].entity;
                                });
                            if (it == vecToDraw.end())
                                vecToDraw.push_back(current->m_entity[i].entity);

                        }

                        auto aabb = current->aabb;
                        auto size = aabb.Size();
                        KT::Vector2F origin = size / 2.0f;
                        auto pos = aabb.Center();
                        auto rec = sf::RectangleShape({ size.x,size.y });
                        rec.setOrigin({ origin.x,origin.y });
                        rec.setFillColor(sf::Color::Transparent);
                        rec.setOutlineThickness(1);
                        rec.setOutlineColor(sf::Color::Red);
                        rec.setPosition({ pos.x,pos.y });
                        Quadrec.push_back(rec);

                    }



                });

         



            for (auto d : vecToDraw)
                d->Render(&window);
            for (auto& q : Quadrec)
                window.draw(q);
            window.draw(rec);
            lastFrameRender = RenderDT.GetElapsedTime();
            window.display();
        }
      


    }

    return 0;
}
