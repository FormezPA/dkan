 @api @javascript
  Scenario Outline: As a user with any Workflow role, I should be able to upgrade my own draft content to needs review.
    Given I am logged in as "<user>"
    And datasets:
      | title              | author | moderation | moderation_date | date created  |
      | My Draft Dataset   | <user> | draft      | Jul 21, 2015    | Jul 21, 2015  |
    And resources:
      | title              | dataset             | author | format |  moderation |  moderation_date | date created  |
      | My Draft Resource  | My Draft Dataset    | <user> | csv    |  draft      |  Jul 21, 2015    | Jul 21, 2015  |
    And I am on the "My Drafts" page
    Then I should see the button "Submit for review"
    And I should see "My Draft Dataset"
    And I should see "My Draft Resource"
    And I check the box "Select all items on this page"
    And I press "Submit for review"
    #Then I wait for "Performed Submit for review on 1 item"
    Then I wait for "Performed Submit for review on 2 items."
    Examples:
      | user        |
      | Contributor |
