function MyCtrl($scope, $timeout) {
    $scope.frameName = 'foo';
    $scope.frameUrl = 'NastaveniP.html';
     }
    // The timeout is here to be sure that the DOM is fully loaded.
    // This is a dirty-as-hell example, please use a directive in a real application.
    