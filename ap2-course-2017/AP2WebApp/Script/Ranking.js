

/**
 * ask for list from server then display it
 */
var myApp = angular.module("rankModule", []);
myApp.controller("rankController", function ($scope, $http) {
    $scope.records = [];

    angular.element(document).ready(function () {
        $("#site-header").load("MenuNav.html");

        // make the ajax call
        $http({
            method: 'POST',
            url: 'api/Player/GetPlayers'
        }).then(function successCallback(response) {
            // this callback will be called asynchronously
            // when the response is available
            for (var i = 0; i < response.data.length; ++i) {
                var uname = response.data[i].Name;
                var wins = parseInt(response.data[i].Wins);
                var loss = parseInt(response.data[i].Losses);

                var record = { username: uname, win: wins, loss: loss };
                $scope.records.push(record);
            }
        }, function errorCallback(response) {
            // called asynchronously if an error occurs
            // or server returns response with an error status.
            console.log("bad response.");
        });
    });
});