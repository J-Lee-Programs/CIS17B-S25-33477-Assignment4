#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

//personal notes:
//unordered maps are used for fast lookups with IDs
//ordered maps are used to find their name description (lamp, light, vacuum)

//CHECKLIST:
//StoredItem class attributes [X]
//Exception class implements [X]
//Storage manager class [/] implementation with maps
//Main implement [/] 


class DuplicateItemException : public std::exception {
private:
    std::string message;
public:
    DuplicateItemException(const std::string& msg) : message(msg) {}
    const char* what() const throw() { //Derived class of Exception
        return message.c_str(); //Return the input of constructor
    }
}; //complete

class ItemNotFoundException : public std::exception {
private:
    std::string message;
public:
    ItemNotFoundException(const std::string& msg) : message(msg) {}
    const char* what() const throw() { //Derived class of Exception
        return message.c_str(); //Return the input of constructor
    }
}; //complete

class StoredItem {
private:
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string id, std::string desc, std::string loc)//constructor
        : id(id), description(desc), location(loc) {
    }

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
}; //all good don't touch

//-------------------------------------------------------------------------------------//

class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById; //this item shows based on item id string (i.e. ITEM-004)
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription; //this map shows based on item name string (i.e. LAMP)

public:
    void addItem(const std::shared_ptr<StoredItem>& item) {//add to both maps
        if (itemById.find(item->getId()) != itemById.end()) { //item smart pointer to getid
            throw DuplicateItemException("Item already exists. " + item->getId());
        }
        else {
            itemById[item->getId()] = item; //pass item to itembyid...
            itemByDescription[item->getDescription()] = item; //pass item to itembydesc...
        }
    }

    std::shared_ptr<StoredItem> findById(const std::string& id) const { 
        //if the find function does NOT reach the end of the container then return the item it found 
        if (itemById.find(id) != itemById.end()) {
            return itemById.find(id)->second;
        }
        else {
            throw ItemNotFoundException("Item not found: " + id);
        }
   
    }

    void removeItem(const std::string& id) {
        //find id, get the member
        auto it = itemById.find(id);
        std::string desc = it->second->getDescription();

        if (it == itemById.end()) { //if find id reaches the end of it's container, throw excep
            throw ItemNotFoundException("Item does not exist: " + id);
        }
        else {
            itemById.erase(it);
            itemByDescription.erase(desc);
        }
    }

    void listItemsByDescription() const { //give list of items unordered
        for (auto& pair : itemByDescription) { //declare a list of the items and go through that container
            //the "second" is part of the storeditem container and points to their members
            std::cout << "ID: " << pair.second->getId()
                << ", Description: " << pair.second->getDescription()
                << ", Location: " << pair.second->getLocation()
                << std::endl;
        }

    }
};

void testDuplicateAddition() {
    //call in main to trigger adding a duplicate item
    StorageManager Storage;
    auto item5 = std::make_shared<StoredItem>("ITEM-005", "Siren light", "Aisle 5, Shelf 1");
    Storage.addItem(item5);
    Storage.addItem(item5);
    
}
void testItemNotFound() {
    //call in main to trigger removing or finding a non existent item
    StorageManager Storage;
    Storage.findById("ITEM-007");
}

int main() {
    try {
        //create items
        StorageManager Storage;
        auto item1 = std::make_shared<StoredItem>("ITEM-001", "Light bulb", "Aisle 5, Shelf 1");
        auto item2 = std::make_shared<StoredItem>("ITEM-002", "Copper wire", "Aisle 6, Shelf 3");
        auto item3 = std::make_shared<StoredItem>("ITEM-003", "Extension cord", "Aisle 7, Shelf 2");
        //adding item
        std::cout << "Adding item: " << item1->getDescription() << std::endl;
        std::cout << "Adding item: " << item2->getDescription() << std::endl;
        std::cout << "Adding item: " << item3->getDescription() << std::endl;
        Storage.addItem(item1);
        Storage.addItem(item2);
        Storage.addItem(item3);

        //find item
        std::cout << std::endl << "Finding item: " << item1->getId() << std::endl;
        auto finding = Storage.findById("ITEM-001");
        std::cout << "Item found: " << finding->getDescription() << std::endl;

        //remove item
        std::cout << std::endl << "Removing item: " << item1->getId() << std::endl;
        Storage.removeItem("ITEM-001");
        std::cout << "Item " << finding->getId() << " removed." << std::endl;

        //list all items
        std::cout << std::endl << "Listing all items: " << std::endl;
        Storage.listItemsByDescription();

        //test dupe
        std::cout << std::endl << "Testing for duplicate addition" << std::endl;
        testDuplicateAddition();

        //test notfound
        std::cout << "Testing for item unfound" << std::endl;
        testItemNotFound();
        //*this works, just comment out test dupe and it'll throw
    }
    catch (DuplicateItemException& e) {
        std::cerr << "Error caught for reason: " << e.what() << std::endl;
    }
    catch (ItemNotFoundException& e) {
        std::cerr << "Error in system: " << e.what() << std::endl;
    }
}