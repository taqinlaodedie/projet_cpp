#include "role.hh"

class PlayerRole: public Role
{
public:
	PlayerRole(int x, int y, RoleType type, std::vector<SDL_Texture*>& vec);
	virtual void draw(SDL_Renderer* renderer);
	virtual ~PlayerRole();
private:
	void setTextures(std::vector<SDL_Texture*>& vec);
};