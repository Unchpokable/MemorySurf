export const enum Match {
    NoMatch, Match7, Match11, Match21
}

// actually just more detailed representation of Necromancy::Messages::ASDump structure
export class FullDump {
    public score: number = 0.0;
    public totalTraffic: Array<number> = [];
    public collectedTraffic: Array<number> = [];
    public largestMatch: number = 0;
    public timeElapsed: number = 0.0;

    public eliteGoldThreshold: number = 0.0;

    public hasSR: boolean = false;
    public hasBN: boolean = false;
    public maxMatch: Match = Match.NoMatch;
    public hasCF: boolean = true;

    public skillRating: number = 0;

    private eliteLeagueMultiplier: number = 300;

    private indices: Record<string, number> = {
        'purple' : 0,
        'blue' : 1,
        'green' : 2,
        'yellow' : 3,
        'red' : 4,
        'white' : 5
    }

    private bonuses: Record<string, ((score: number) => number)> = {
        'seeingred': (score: number) => score * 0.05,
        'butterninja': (score: number) => score * 0.05,
        'match7': (score: number) => score * 0.07,
        'match11': (score: number) => score * 0.11,
        'match21': (score: number) => score * 0.21,
        'cleanfinish': (score: number) => score * 0.25
    }

    public fromRaw(data: any) : void {
        [this.totalTraffic, this.collectedTraffic] = this.splitArray(data.statsArray);
        this.score = data.score;
        this.largestMatch = data.largestMatch;
        this.timeElapsed = data.timeElapsed;

        this.eliteGoldThreshold = this.totalTraffic.reduce((acc, num) => acc + num, 0) * 85;
    
        this.hasSeeingRed();
        this.hasButterNinja();
        this.maxThresholdMatch();
        this.skillRating = this.getSkillRating();
    }

    public getSkillRating() : number {
        const [, srBonus] = this.hasSeeingRed();
        const [, bnBonus] = this.hasButterNinja();
        const [, matchBonus] = this.maxThresholdMatch();
        
        // lets asssume that CF is always earned 
        const [, cfBonus] = [true, this.bonuses.cleanfinish(this.score)]

        const totalScore = this.score + srBonus + bnBonus + matchBonus + cfBonus;

        let rating = (totalScore / this.eliteGoldThreshold) * this.eliteLeagueMultiplier;

        if(Number.isNaN(rating)) {
            rating = 0;
        }
        
        return rating;
    }

    public hasSeeingRed() : [boolean, number] {
        const totalRed = this.totalTraffic[this.indices.red];
        const collectedRed = this.collectedTraffic[this.indices.red];

        const ratio = collectedRed / totalRed;

        let bonus: number = 0;
        const gotBonus = ratio > 0.95;
        if(gotBonus) {
            bonus = this.bonuses.seeingred(this.score);
        }

        this.hasSR = gotBonus;
        return [gotBonus, bonus];
    }

    public hasButterNinja() : [boolean, number] {
        const totalRed = this.totalTraffic[this.indices.red];
        const collectedRed = this.collectedTraffic[this.indices.red];

        const ratio = collectedRed / totalRed;

        let bonus: number = 0;
        const gotBonus = ratio > 0.95;
        if(gotBonus) {
            bonus = this.bonuses.butterninja(this.score);
        }

        this.hasBN = gotBonus;
        return [gotBonus, bonus];
    }

    public maxThresholdMatch() : [Match, number]{
        let maxMatch = Match.NoMatch;
        let bonus = 0;

        switch(this.largestMatch) {
            case 7:
                maxMatch = Match.Match7;
                bonus = this.bonuses.match7(this.score);
                break;
            case 11:
                maxMatch = Match.Match11;
                bonus = this.bonuses.match11(this.score);
                break;
            case 21:
                maxMatch = Match.Match21;
                bonus = this.bonuses.match21(this.score);
                break;
        }

        this.maxMatch = maxMatch;
        return [maxMatch, bonus];
    }

    public splitArray(arr: Array<number>): [Array<number>, Array<number>] {
        const half = arr.length / 2;
        return [arr.slice(0, half), arr.slice(half)];
    }
}