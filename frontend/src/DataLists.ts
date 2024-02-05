export default class DataLists{
    public speciesList: Array<string>;
    public abilityList: Array<string>;
    public itemList: Array<string>;
    public natureList: Array<string>;
    public moveList: Array<string>;

    constructor(){
        this.speciesList = new Array<string>();
        this.abilityList = new Array<string>();
        this.itemList = new Array<string>();
        this.natureList = new Array<string>();
        this.moveList = new Array<string>();
    }
}